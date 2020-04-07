#pragma once

#include <QHash>
#include <QString>
#include <QVariant>

namespace ui
{
/**
*	@brief Stores the state for a single control panel
*/
class ControlPanelState final
{
public:
	ControlPanelState() = default;

	ControlPanelState(const ControlPanelState&) = delete;
	ControlPanelState& operator=(const ControlPanelState&) = delete;

	~ControlPanelState() = default;

	QVariant Value(const QString& key, const QVariant& defaultValue = {}) const
	{
		if (auto it = _values.find(key); it != _values.end())
		{
			return it.value();
		}

		return defaultValue;
	}

	void SetValue(const QString& key, const QVariant& value)
	{
		_values.insert(key, value);
	}

	void Clear()
	{
		_values.clear();
	}

private:
	QHash<QString, QVariant> _values;
};
}
