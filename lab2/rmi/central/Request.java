package rmi.central;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import rmi.weather.*;
import rmi.horoscope.*;
import java.rmi.registry.*;
public class Request extends UnicastRemoteObject implements RequestInterface {
	public Request() throws RemoteException {
		super();
	}

	public String[] request(String entry) {
		String[] entrys = entry.split(" "), predictions = new String[2];

		try {
			Registry myRegistryWeather = LocateRegistry.getRegistry("127.0.0.1", 9090);
			WeatherInterface server_weather = (WeatherInterface)
			myRegistryWeather.lookup("ServerWeather");

			Registry myRegistryHoroscope = LocateRegistry.getRegistry("127.0.0.1", 9091);
			HoroscopeInterface server_horoscope = (HoroscopeInterface)
			myRegistryHoroscope.lookup("ServerHoroscope");

			predictions[0] = server_weather.consultWeather(entrys[0]);
			predictions[1] = server_horoscope.consultHoroscope(entrys[1]);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return predictions;
	}
}
