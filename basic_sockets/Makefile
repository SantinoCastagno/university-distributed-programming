
all: 
	gcc src/client.c src/utils.c -o bin/c
	gcc src/server_central.c src/dictionary.c src/utils.c -o bin/sc
	gcc src/server_horoscope.c src/utils.c -o bin/sh
	gcc src/server_weather.c src/utils.c -o bin/sw

clean:
	rm bin/c bin/sc bin/sh bin/sw
