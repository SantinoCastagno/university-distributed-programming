package rmi.central;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.*;
import rmi.weather.*;
import rmi.horoscope.*;

public class CentralObject extends UnicastRemoteObject implements CentralInterface {

	public CentralObject() throws RemoteException {
		super();
	}

	public String[] requestCentral(String entry) {
		String[] entrys = entry.split(" "), predictions = new String[2];

		try {
			Registry myRegistryWeather = LocateRegistry.getRegistry("127.0.0.1", 9090);
			WeatherInterface server_weather = (WeatherInterface) myRegistryWeather.lookup("WeatherObject");

			Registry myRegistryHoroscope = LocateRegistry.getRegistry("127.0.0.1", 9091);
			HoroscopeInterface server_horoscope = (HoroscopeInterface) myRegistryHoroscope.lookup("HoroscopeObject");

			predictions[0] = server_weather.requestWeather(entrys[0]);
			predictions[1] = server_horoscope.requestHoroscope(entrys[1]);
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
