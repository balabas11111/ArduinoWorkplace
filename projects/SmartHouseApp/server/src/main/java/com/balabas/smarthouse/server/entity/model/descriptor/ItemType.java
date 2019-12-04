package com.balabas.smarthouse.server.entity.model.descriptor;

public enum ItemType {
	DEVICE(1000 * 60 * 2, "Устройство", Emoji.PAGER, "device"),
	SENSORS(1000 * 60 * 1, "Датчики", Emoji.JAPANESE_SYMBOL_FOR_BEGINNER, "sensors"),
	SETTINGS(1000 * 60 * 5, "Настройки", Emoji.HAMMER_AND_WRENCH, "settings"),
	SERVICES(1000 * 60 * 2, "Сервисы", Emoji.BRIEFCASE, "services"),
	CUSTOM(-1, "", Emoji.BRIEFCASE, "custom"),
	ENTITY(-1, "", Emoji.BRIEFCASE, "entity");

	long refreshInterval;
	String description;
	Emoji emoji;
	String code;

	ItemType(long refreshInterval, String description, Emoji emoji, String code) {
		this.refreshInterval = refreshInterval;
		this.description = description;
		this.emoji = emoji;
		this.code = code;
	}

	public long getRefreshInterval() {
		return this.refreshInterval;
	}

	public String getDescription() {
		return description;
	}

	public Emoji getEmoji() {
		return emoji;
	}
	
	public String getCode() {
		return code;
	}

	public static ItemType getItemTypeByName(String name) {
		for (ItemType gt : ItemType.values()) {
			if (gt.name().equalsIgnoreCase(name)) {
				return gt;
			}
		}
		return ItemType.CUSTOM;
	}
}
