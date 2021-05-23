#pragma once

#include "../util/icon.hpp"
#include "lib/settings.hpp"
#include "settingspage.hpp"
#include "view/logview.hpp"

class ApplicationPage: public SettingsPage
{
public:
	ApplicationPage(lib::settings &settings, QWidget *parent);

	auto icon() -> QIcon override;
	auto title() -> QString override;
	auto save() -> bool override;

private:
	QCheckBox *appMedia = nullptr;
	QCheckBox *appPulse = nullptr;
	QCheckBox *appWhatsNew = nullptr;
	QComboBox *appRefresh = nullptr;
	QComboBox *appMaxQueue = nullptr;

	static constexpr int minRefreshInterval = 1;
	static constexpr int maxRefreshInterval = 60;

	static constexpr int minMaxQueue = 1;
	static constexpr int maxMaxQueue = 1000;

	static auto isPulse() -> bool;

	auto app() -> QWidget *;
	auto appLogs() -> QWidget *;
};


