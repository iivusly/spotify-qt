#include "spotify.hpp"

using namespace spt;

Spotify::Spotify()
{
	lastAuth = 0;
	networkManager = new QNetworkAccessManager();
	refresh();
}

Spotify::~Spotify()
{
	delete networkManager;
}

QNetworkRequest Spotify::request(const QString &url)
{
	// See when last refresh was
	auto lastRefresh = QDateTime::currentSecsSinceEpoch() - lastAuth;
	if (lastRefresh >= 3600)
	{
		qDebug() << "access token probably expired, refreshing";
		refresh();
	}
	// Prepare request
	QNetworkRequest request((QUrl("https://api.spotify.com/v1/" + url)));
	// Set header
	request.setRawHeader("Authorization", ("Bearer " + Settings().accessToken()).toUtf8());
	// Return prepared header
	return request;
}

quint32 Spotify::get(const QString &url)
{
	// ID for this request
	auto id = randomId();
	// Prepare signal
	QMetaObject::Connection connection;
	connection = QNetworkAccessManager::connect(networkManager, &QNetworkAccessManager::finished,
		[this, id, connection, url](QNetworkReply *reply) {
		// Parse reply as json
		auto json = QJsonDocument::fromJson(reply->readAll());
		reply->deleteLater();
		// Check for errors
		if (!json["error"].isNull())
			emit error(QString("%1 failed: %2")
				.arg(url)
				.arg(json["error"].toObject()["message"].toString()));
		else
			emit jsonResponse(id, json);
		// Disconnect
		QNetworkAccessManager::disconnect(connection);
	});
	// Send request
	networkManager->get(request(url));
	// Return id to remember request
	return id;
}

QString Spotify::put(QString url, QVariantMap *body)
{
	// Set in header we're sending json data
	auto req = request(url);
	req.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
	// Send the request, we don't expect any response
	auto putData = body == nullptr ? nullptr : QJsonDocument::fromVariant(*body).toJson();
	return errorMessage(networkManager->put(req, putData));
}

QString Spotify::post(QString url)
{
	auto req = request(url);
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	return errorMessage(networkManager->post(req, QByteArray()));
}

QString Spotify::del(QString url, QVariantMap *body)
{
	auto req = request(url);
	req.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
	auto delData = body == nullptr ? nullptr : QJsonDocument::fromVariant(*body).toJson();
	return errorMessage(networkManager->sendCustomRequest(req, "DELETE", delData));
}

QString Spotify::errorMessage(QNetworkReply *reply)
{
	while (!reply->isFinished())
		QCoreApplication::processEvents();
	auto replyBody = reply->readAll();
	reply->deleteLater();
	if (replyBody.isEmpty())
		return QString();
	auto json = QJsonDocument::fromJson(replyBody);
	return json["error"].toObject()["message"].toString();
}

bool Spotify::refresh()
{
	// Make sure we have a refresh token
	Settings settings;
	auto refreshToken = settings.refreshToken();
	if (refreshToken.isEmpty())
	{
		qWarning() << "warning: attempt to refresh without refresh token";
		return false;
	}
	// Create form
	auto postData = QString("grant_type=refresh_token&refresh_token=%1")
		.arg(refreshToken)
		.toUtf8();
	// Create request
	QNetworkRequest request(QUrl("https://accounts.spotify.com/api/token"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	request.setRawHeader(
		"Authorization",
		"Basic " + QString("%1:%2")
		.arg(settings.clientId()).arg(settings.clientSecret()).toUtf8().toBase64());
	// Send request
	auto reply = networkManager->post(request, postData);
	while (!reply->isFinished())
		QCoreApplication::processEvents();
	// Parse json
	auto json = QJsonDocument::fromJson(reply->readAll()).object();
	reply->deleteLater();
	// Check if error
	if (json.contains("error_description"))
	{
		qWarning() << "warning: failed to refresh token:" << json["error_description"];
		return false;
	}
	// Save as access token
	lastAuth = QDateTime::currentSecsSinceEpoch();
	auto accessToken = json["access_token"].toString();
	settings.setAccessToken(accessToken);
	return true;
}

void Spotify::getPlaylists()
{
	// Request playlists
	auto reqId = get("me/playlists?limit=50");
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		// Parse as playlists
		auto items = json["items"].toArray();
		// Create list of playlists
		auto size = json["total"].toInt();
		QVector<Playlist> playlists;
		playlists.reserve(size);
		for (int i = 0; i < items.size(); i++)
			playlists.insert(i, Playlist(items.at(i).toObject()));
		// Emit list of playlists
		emit playlistsResponse(playlists);
		Spotify::disconnect(connection);
	});
}

QVector<Device> Spotify::devices()
{
	auto reqId = get("me/player/devices");
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		auto items = json["devices"].toArray();
		QVector<Device> devices(items.size());
		for (int i = 0; i < items.size(); i++)
		{
			auto data = items.at(i).toObject();
			Device device;
			device.id				= data["id"].toString();
			device.name				= data["name"].toString();
			device.type				= data["type"].toString();
			device.isActive			= data["is_active"].toBool();
			device.isPrivateSession	= data["is_private_session"].toBool();
			device.isRestricted		= data["is_restricted"].toBool();
			device.volumePercent	= data["volume_percent"].toInt();
			devices[i] = device;
		}
		emit devicesResponse(devices);
		Spotify::disconnect(connection);
	});
}

QString Spotify::setDevice(Device device)
{
	QVariantMap body;
	body["device_ids"] = QStringList({
		device.id
	});
	currentDevice = device.id;
	return put("me/player", &body);
}

QString Spotify::playTracks(const QString &track, const QString &context)
{
	QVariantMap body;
	body["context_uri"] = context;
	body["offset"] = QJsonObject({
		QPair<QString, QJsonValue>("uri", track)
	});
	return put(currentDevice == nullptr
		? QString("me/player/play")
		: QString("me/player/play?device_id=%1").arg(currentDevice), &body);
}

QString Spotify::playTracks(const QString &track, const QStringList &all)
{
	QVariantMap body;
	body["uris"] = all;
	body["offset"] = QJsonObject({
		QPair<QString, QJsonValue>("uri", track)
	});
	return put(currentDevice == nullptr
		? QString("me/player/play")
		: QString("me/player/play?device_id=%1").arg(currentDevice), &body);
}

QString Spotify::playTracks(const QString &context)
{
	QVariantMap body;
	body["context_uri"] = context;
	return put(currentDevice == nullptr
		? QString("me/player/play")
		: QString("me/player/play?device_id=%1").arg(currentDevice), &body);
}

QString Spotify::setShuffle(bool enabled)
{
	return put(QString("me/player/shuffle?state=%1").arg(enabled ? "true" : "false"));
}

void Spotify::getCurrentPlayback()
{
	auto reqId = get("me/player");
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		Playback playback;
		if (json["item"].isNull())
		{
			// No track playing
			playback.progressMs	= 0u;
			playback.item 		= new Track();
			playback.isPlaying 	= false;
			playback.volume 	= 100;
			playback.repeat 	= "off";
			playback.shuffle	= false;
		}
		else
		{
			playback.progressMs	= json["progress_ms"].toInt();
			playback.item 		= new Track(json["item"].toObject());
			playback.isPlaying 	= json["is_playing"].toBool();
			playback.volume 	= json["device"].toObject()["volume_percent"].toInt();
			playback.repeat		= json["repeat_state"].toString();
			playback.shuffle	= json["shuffle_state"].toBool();
		}
		emit currentPlaybackResponse(playback);
		Spotify::disconnect(connection);
	});
}

QString Spotify::pause()
{
	return put("me/player/pause");
}

QString Spotify::resume()
{
	return put("me/player/play");
}

QString Spotify::seek(int position)
{
	return put(QString("me/player/seek?position_ms=%1").arg(position));
}

QString Spotify::next()
{
	return post("me/player/next");
}

QString Spotify::previous()
{
	return post("me/player/previous");
}

QString Spotify::setVolume(int volume)
{
	return put(QString("me/player/volume?volume_percent=%1").arg(volume));
}

QString Spotify::setRepeat(QString state)
{
	return put(QString("me/player/repeat?state=%1").arg(state));
}

void Spotify::getTrackAudioFeatures(QString trackId)
{
	auto reqId = get(QString("audio-features/%1")
		.arg(trackId.startsWith("spotify:track:")
			? trackId.remove(0, QString("spotify:track:").length())
			: trackId));
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		emit audioFeaturesResponse(AudioFeatures(json.object()));
		Spotify::disconnect(connection);
	});
}

void Spotify::getAlbumTracks(const QString &albumID)
{
	auto reqId = get(QString("albums/%1").arg(albumID));
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		auto albumName = json["name"].toString();
		QVector<Track> tracks;
		tracks.reserve(json["total_tracks"].toInt());
		// Loop through all items
		for (auto track : json["tracks"].toObject()["items"].toArray())
		{
			auto t = Track(track.toObject());
			// Album name is not included, so we have to set it manually
			t.album = albumName;
			tracks.append(t);
		}
		// Emit final vector
		emit albumTracksResponse(tracks);
		Spotify::disconnect(connection);
	});
}

void Spotify::getArtist(const QString &artistId)
{
	auto reqId = get(QString("artists/%1").arg(artistId));
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		emit artistResponse(Artist(json.object()));
		Spotify::disconnect(connection);
	});
}

void Spotify::getPlaylist(const QString &playlistId)
{
	auto reqId = get(QString("playlists/%1").arg(playlistId));
	QMetaObject::Connection connection;
	connection = Spotify::connect(this, &Spotify::jsonResponse,
		[this, reqId, connection](quint32 id, const QJsonDocument &json) {
		if (reqId != id)
			return;
		emit playlistResponse(Playlist(json.object()));
		Spotify::disconnect(connection);
	});
}

QString Spotify::addToPlaylist(const QString &playlistId, const QString &trackId)
{
	return post(QString("playlists/%1/tracks?uris=%2")
		.arg(playlistId).arg(trackId));
}

QString Spotify::removeFromPlaylist(const QString &playlistId, const QString &trackId, int pos)
{
	QVariantMap body;
	body["tracks"] = QJsonArray({
		QJsonObject({
			QPair<QString, QString>("uri", trackId),
			QPair<QString, QJsonArray>("positions", QJsonArray({
				pos
			}))
		})
	});
	return del(QString("playlists/%1/tracks").arg(playlistId), &body);
}

quint32 Spotify::randomId()
{
	return QRandomGenerator::global()->generate();
}