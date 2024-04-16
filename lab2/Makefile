# Compiles all the .java
all:
	javac rmi/central/*.java
	javac rmi/client/*.java
	javac rmi/weather/*.java
	javac rmi/horoscope/*.java

# Run the horoscope object
horoscope_object:
	java rmi.horoscope.HoroscopeObject

# Run the weather object
weather_object:
	java rmi.weather.WeatherObject

# Run the central object
central_object:
	java rmi.central.CentralObject

# Run the client
client_object:	
	java rmi.client.ClientObject

clean:
	rm rmi/weather/*.class
	rm rmi/horoscope/*.class
	rm rmi/central/*.class
	rm rmi/client/*.class