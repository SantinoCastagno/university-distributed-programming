# Compiles all the .java
all:
	javac rmi/central/*.java
	javac rmi/client/*.java
	javac rmi/weather/*.java
	javac rmi/horoscope/*.java

# Run the horoscope object
horoscope_object:
	java rmi.horoscope.ServerHoroscope

# Run the weather object
weather_object:
	java rmi.weather.ServerWeather

# Run the central object
central_object:
	java rmi.central.ObjectCentral

# Run the client
client:	
	java rmi.client.Client

clean:
	rm rmi/central/*.class
	rm rmi/client/*.class