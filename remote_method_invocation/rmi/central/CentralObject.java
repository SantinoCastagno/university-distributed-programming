package rmi.central;

import java.util.HashMap;
import java.util.Map;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.util.concurrent.Semaphore;
import java.rmi.registry.*;
import rmi.weather.*;
import rmi.horoscope.*;
import java.util.Properties;
import java.io.FileInputStream;

public class CentralObject extends UnicastRemoteObject implements CentralInterface {

	private static Map<String, String> cache = new HashMap<>();
	private static Semaphore semaphoreHoroscope = new Semaphore(1);
	private static Semaphore semaphoreWeather = new Semaphore(1);

	public CentralObject() throws RemoteException {
		super();
	}

	public String[] requestCentral(String entry) {
		String weather = "", horoscope = "";
		String[] entrys = entry.split(" "), predictions = new String[2];
		String cache_aux_1, cache_aux_2;
		String IP = "", IP_HOR = "", IP_WEA = "";
		int PORT_HOR, PORT_WEA;
		Properties properties = new Properties();

		try {
			properties.load(new FileInputStream("rmi/central/config.properties"));
			IP = properties.getProperty("IP");

			PORT_HOR = Integer.parseInt(properties.getProperty("PORT_HOR_SERVER"));
			IP_HOR = properties.getProperty("IP_HOR_SERVER");

			PORT_WEA = Integer.parseInt(properties.getProperty("PORT_WEA_SERVER"));
			IP_WEA = properties.getProperty("IP_WEA_SERVER");

			Registry myRegistryWeather = LocateRegistry.getRegistry(IP_WEA, PORT_WEA);
			WeatherInterface server_weather = (WeatherInterface) myRegistryWeather.lookup("WeatherObject");

			Registry myRegistryHoroscope = LocateRegistry.getRegistry(IP_HOR, PORT_HOR);
			HoroscopeInterface server_horoscope = (HoroscopeInterface) myRegistryHoroscope.lookup("HoroscopeObject");

			// Checks the cache for the horoscope
			semaphoreHoroscope.acquire();
			cache_aux_1 = cache.get(entrys[0]);
			semaphoreHoroscope.release();

			if (cache_aux_1 == null) {
				// If the horoscope is not in the cache, it is requested from the server
				horoscope = server_horoscope.requestHoroscope(entrys[0]);
				predictions[0] = horoscope;
				semaphoreHoroscope.acquire();
				cache.put(entrys[0], horoscope);
				semaphoreHoroscope.release();
			} else {
				// If the horoscope is in the cache, it is saved for return
				semaphoreHoroscope.acquire();
				predictions[0] = cache_aux_1;
				semaphoreHoroscope.release();
			}
	
			// Checks the cache for the weather
			semaphoreWeather.acquire();
			cache_aux_2 = cache.get(entrys[1]);
			semaphoreWeather.release();

			if (cache_aux_2 == null) {
				// If the weather is not in the cache, it is requested from the server
				weather = server_weather.requestWeather(entrys[1]);
				predictions[1] = weather;
				semaphoreWeather.acquire();
				cache.put(entrys[1], weather);
				semaphoreWeather.release();
			} else {
				// If the weather is in the cache, it is saved for return
				semaphoreWeather.acquire();
				predictions[1] = cache.get(entrys[1]);
				semaphoreWeather.release();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return predictions;
	}

	public static void main(String[] args) {
		Properties properties = new Properties();
		int PORT;
		try {
			properties.load(new FileInputStream("rmi/central/config.properties"));
			PORT = Integer.parseInt(properties.getProperty("PORT"));

			Registry myRegistry = LocateRegistry.createRegistry(PORT);
			myRegistry.rebind("CentralObject", new CentralObject());
			System.out.println("LOG: Central server ON");
		} catch (Exception e) {
			System.err.println("Error in client: " + e.getMessage());
		}
	}

}
