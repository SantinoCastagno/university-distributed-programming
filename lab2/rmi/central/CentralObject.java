package rmi.central;

import java.util.HashMap;
import java.util.Map;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.*;
import rmi.weather.*;
import rmi.horoscope.*;

public class CentralObject extends UnicastRemoteObject implements CentralInterface {

	private static Map<String, String> cache = new HashMap<>();

	public CentralObject() throws RemoteException {
		super();
	}

	public String[] requestCentral(String entry) {
		String weather = "", horoscope = "";
		String[] entrys = entry.split(" "), predictions = new String[2];

		try {
			Registry myRegistryWeather = LocateRegistry.getRegistry("127.0.0.1", 9090);
			WeatherInterface server_weather = (WeatherInterface) myRegistryWeather.lookup("WeatherObject");

			Registry myRegistryHoroscope = LocateRegistry.getRegistry("127.0.0.1", 9091);
			HoroscopeInterface server_horoscope = (HoroscopeInterface) myRegistryHoroscope.lookup("HoroscopeObject");

			if (cache.get(entrys[0]) == null) {
				horoscope = server_horoscope.requestHoroscope(entrys[0]);
				predictions[0] = horoscope;
				cache.put(entrys[0], horoscope);
			} else {
				predictions[0] = cache.get(entrys[0]);
			}

			if (cache.get(entrys[1]) == null) {
				weather = server_weather.requestWeather(entrys[1]);
				predictions[1] = weather;
				cache.put(entrys[1], weather);
			} else {
				predictions[1] = cache.get(entrys[1]);
			}

			// predictions[0] = server_weather.requestWeather(entrys[0]);
			// predictions[1] = server_horoscope.requestHoroscope(entrys[1]);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return predictions;
	}

	public static void main(String[] args) {
		try {
			Registry myRegistry = LocateRegistry.createRegistry(8080);
			myRegistry.rebind("CentralObject", new CentralObject());
			System.out.println("LOG: Central server ON");
		} catch (Exception e) {
			System.err.println("Error in client: " + e.getMessage());
		}
	}

}
