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
		String IP = "", PORT_HOR_String = "", PORT_WEA_String = "";
		int PORT_HOR, PORT_WEA;
		Properties properties = new Properties();

		try {
			properties.load(new FileInputStream("rmi/central/config.properties"));
			IP = properties.getProperty("IP");

			PORT_HOR_String = properties.getProperty("PORT_HOR_SERVER");
			PORT_HOR = Integer.parseInt(PORT_HOR_String);

			PORT_WEA_String = properties.getProperty("PORT_WEA_SERVER");
			PORT_WEA = Integer.parseInt(PORT_WEA_String);

			Registry myRegistryWeather = LocateRegistry.getRegistry(IP, PORT_HOR);
			WeatherInterface server_weather = (WeatherInterface) myRegistryWeather.lookup("WeatherObject");

			Registry myRegistryHoroscope = LocateRegistry.getRegistry(IP, PORT_WEA);
			HoroscopeInterface server_horoscope = (HoroscopeInterface) myRegistryHoroscope.lookup("HoroscopeObject");

			semaphoreHoroscope.acquire();
			cache_aux_1 = cache.get(entrys[0]);
			semaphoreHoroscope.release();

			if (cache_aux_1 == null) {
				horoscope = server_horoscope.requestHoroscope(entrys[0]);
				predictions[0] = horoscope;
				semaphoreHoroscope.acquire();
				cache.put(entrys[0], horoscope);
				semaphoreHoroscope.release();
			} else {
				semaphoreHoroscope.acquire();
				predictions[0] = cache_aux_1;
				semaphoreHoroscope.release();
			}

			semaphoreWeather.acquire();
			cache_aux_2 = cache.get(entrys[1]);
			semaphoreWeather.release();
			if (cache_aux_2 == null) {
				weather = server_weather.requestWeather(entrys[1]);
				predictions[1] = weather;
				semaphoreWeather.acquire();
				cache.put(cache_aux_2, weather);
				semaphoreWeather.release();
			} else {
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
		try {
			Registry myRegistry = LocateRegistry.createRegistry(8080);
			myRegistry.rebind("CentralObject", new CentralObject());
			System.out.println("LOG: Central server ON");
		} catch (Exception e) {
			System.err.println("Error in client: " + e.getMessage());
		}
	}

}
