APP_NAME := weather

CROSS_COMPILE = arm-openwrt-linux-

all: $(APP_NAME)
	cp $(APP_NAME) /home/test/share

$(APP_NAME): weather.c
	$(CROSS_COMPILE)gcc $(APP_NAME).c -o $(APP_NAME)


