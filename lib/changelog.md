Versions use major.minor scheming. New versions are released together with spotify-qt.

* Major releases contain breaking changes and may require changes to be made.
* Minor releases contain minor changes and bug fixes and should never require changes to be made.
* Pre-release and beta releases may contain breaking changes, even if it's a minor release.

## v0.10 (spotify-qt v3.12)
* `album_group`, `album_shape`, `artist_profile` enums now explicitly uses `unsigned char`.
* `client_type`, `experiment`, `follow_type` enums now explicitly uses `unsigned char`.
* `library_layout`, `log_type`, `media_type` enums now explicitly uses `unsigned char`.
* `player_action`, `position`, `repeat_mode` enums now explicitly uses `unsigned char`.
* `resize_mode` enum now explicitly uses `unsigned char`.
* Implemented `enum_to_string` for `album_group`.
* `spt::api::album_tracks` is now paged.
* `spt::api::albums` is now paged.
* `spt::api::new_releases` is now paged.
* `spt::api::followed_artists` is now paged.
* `spt::api::saved_albums` is now paged.
* `spt::api::playlists` is now paged.
* Added `qt.track_title`.
* Added `spt::api::add_saved_albums`, `spt::api::removed_saved_album` and `spt::api::is_saved_album`.
* Removed `audio_feature`, `audio_key`, `audio_mode` enums.
* Removed `spotify.username`.
* Removed `spt::api::track_audio_features`.
* Removed `spt::audio_feature` and `spt::audio_features`.
* Removed `spt::show::available_markets`.
* Removed `USE_KEYCHAIN` support.

## v0.9 (spotify-qt v3.11)
* `player_action` now implements `std::hash`.
* `spt::api::saved_tracks`, `spt::api::playlist_tracks` now expects a paged callback.
* Added `album_shape`, `library_layout`, `position` enums.
* Added `general.ignore_unavailable_index`.
* Added `qt.library_layout`.
* Added `qt.toolbar_position`.
* Added `qt.album_shape`.
* Added `spotify.additional_arguments`.
* Added `spt::api::queue`.
* Added `paged_callback`.
* Added `spt::page`.
* Added `spt::playback::is_valid()`.
* Added `spt::queue`.
* Added `spt::request::get_page`.
* Added `spt::to_relative_url()`.
* Added `developer_mode::add_experiment` and `developer_mode::is_experiment_enabled`.
* Removed `spt::playlist::tracks_href`.

## v0.8 (spotify-qt v3.10)
* `crash_info::timestamp` is now an `unsigned long`.
* `account.last_refresh` is now an `unsigned long`.
* `spt::api` now requires an `spt::request` instance.
* `spt::api::current_playback` now returns a `result`.
* `spt::api::add_to_queue` now returns a `result`.
* `format::size` now takes an `unsigned long`.
* `vector::sub` now properly expects length instead of index.
* Replaced `lib::lyrics` with reworked `lrc::api`.
* Moved `spt::api::select_device` to `spt::device_select`.
* Moved `spt::api::to_uri` to `spt::util::id_to_uri`.
* Moved `spt::api::to_id` to `spt::util::uri_to_id`.
* Moved `spt::api::url_to_uri` to `spt::util::url_to_uri`.
* Moved `spt::api::get_device_url` to `spt::util::get_device_url`.
* Added `player_action` enum.
* Added `general.native_window`.
* Added `general.media_hotkeys` (Windows only).
* Added `spotify.volume`.
* Added `spt::playback::is_allowed`.
* Added `spt::playback::disallowed_actions`.
* Added `spt::request`.
* Added `format::time_pretty`.
* Added `format::kilo`, `format::mega` and `format::giga`.
* Added `http_client::get` and `http_client::post` with `result`.
* Added `strings::erase_non_alpha`.
* Added `lib::time` constants.
* Added `vector::unique`.
* Added `third_party/keychain`.
* Removed `window_system` enum.
* Removed `general.fixed_width_time`.
* Removed `general.last_volume`.
* Removed `spotify.keyring_password`.
* Removed `system::window_system`.

## v0.7 (spotify-qt v3.9)

* `add_saved_track` and `remove_saved_track` now supports multiple tracks.
* `add_to_playlist` and `remove_from_playlist` now supports multiple tracks.
* `spt::track_audio_features` now supports multiple tracks.
* `spt::device` and `spt::playlist` now inherits `spt::entity`.
* `spt::episode` and `spt::show` now supports multiple cover images.
* `spt::track` now supports multiple album images.
* `date_time::seconds_since_epoch` now returns an `unsigned long`.
* Renamed `spt::audio_feature::get_feature` to `get_feature_string`.
* Renamed `spt::audio_feature::get_name` to `get_value_string`.
* Moved, and reworked, string formatting from `lib::format` to `lib::fmt`.
* Reworked `result` into a custom class with ok/fail.
* `ddg::api::search` now returns an empty object on failure.
* Added `album_size` and `device_type` enums.
* Added `general.close_to_tray` and `general.check_for_updates`.
* Added `qt.custom_font_name` and `qt.custom_font_size`.
* Added `qt.album_size`.
* Added `spotify.device_type`.
* Added `spt::api::url_to_uri`.
* Added `spt::audio_feature::get_feature`.
* Added `spt::image` for handling images of multiple sizes.
* Added `spt::track::image_small` and `spt::track::image_large`.
* Added `stopwatch`.
* Added `uri::encode`.
* Added `vector::average`.

## v0.6 (spotify-qt v3.8)

* `palette`, `resize_mode`, and `spotify_context` are now enum classes.
* Renamed `log::dev` to `log::debug`.
* Added `enums` for serializing, and deserializing, enums.
* Added `optional` as a basic `std::optional` implementation.
* Added `uri` for basic URI manipulation.
* Added `gh::api` as a GitHub API wrapper.
* Added `ddg::api` as a DuckDuckGo API wrapper.
* Added `cache::get_album_image_path`.
* Added `cache::get_album` and `cache::set_album`.
* Added `artist_profile`, `audio_quality`, and `media_type` enums.
* Added `json::find_item`, and `json::set`.
* Added `qt.custom_font`, and `qt.mirror_title_bar`.
* Added `spt::api::follow_playlist` and `spt::api::unfollow_playlist`.
* Added `spt::api::is_following_playlist`.
* Added `spt::api::create_playlist`.
* Added `spt::api::show` and `spt::api::show_episodes`.
* Added `spt::episode` and `spt::show`.
* Removed `cipher`.
* Removed `ghc::filesystem` support for `fmt::format`.
* Removed `settings::qt_const` (now dynamically created).
* Removed `settings::validate`, and `settings::set_value`
* Removed `general.pulse_volume` (broken with PipeWire).
* Removed `general.tray_notifications` (use in-app notifications).
* Removed `strings::index_of`, and `strings::last_index_of`.

## v0.5 (spotify-qt v3.7)

* `cache` is now abstract, and previous implementation has been moved to `json_cache`.
* Reworked `audio_feature`.
* Version is now defined in `CMakeLists.txt` (still as `LIB_VERSION`).
* `spt::album` and `spt::artist` now inherits `spt::entity`.
* Added `crash_handler` (GCC amd64 only).
* Added `qt` settings.
* Added `http_client`, and `qt::http_client`.
* Added `spt::auth`, and `qt::spt::auth`.
* Added `spt::playlist::is_up_to_date`.
* Added `track_info` and `lyrics`.
* Added `image`.
* Added `strings::replace_all`.
* Added `lib::system::window_sytem`.
* Added `vector::index_of`.
* Added `qt::system_info`.
* Moved `spotify_error` to `spt::error`.
* Moved `seconds_since_epoch` to `date_time`.
* Moved `lib::qt::sytem` to `lib::system` (no longer depends on Qt).
* Removed `general.show_context_info` (always enabled).
* Removed `vector::remove_if`.

## v0.4 (spotify-qt v3.6)

* Paths in `paths` now return `filesystem::path` instead of `std::string`.
* Setting `general.hidden_song_headers` is now a `std::unordered_set` instead of `std::vector`.
* Version is now defined in `lib/libversion.hpp`.
* Reworked `cache`.
* Added enums: `album_group`, `client_type`, `follow_type`, `repeat_state`.
* Added `general.last_device` setting.
* Added Spotify classes: `album`, `artist`, `audio_features`, `device`, `playback`, `playlist`,
  `search_results`, `track`, `user`.
* Added Spotify helper classes: `context`, `playlist_details`, `saved_album`.
* Added `callback<T>` for asynchronous callbacks.
* Added `entity` type for saving id and name of various entities.
* Added `api` as a Spotify Web API wrapper.
* Added `json`.
* Added `random`.
* Removed deprecated `locale`.

## v0.3 (spotify-qt v3.5)

* All headers are now in an "include" folder.
* `general.song_header_resize_mode` is now `general.track_list_resize_mode`.
* Added `cipher`.
* Added `fmt::time` and `fmt::size`.
* Added `vector`.
* Removed `general.spotify_playback_order`.

## v0.2 (spotify-qt v3.4)

* Added `settings`.
* Added `fmt`.
* Added `log`.
* Added `date_time`.

## v0.1 (spotify-qt v3.3)

* Initial version.