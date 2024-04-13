package rmi;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class centralServer extends UnicastRemoteObject implements server {

  public centralServer() throws RemoteException {
    super();
  }

  @Override
  public String[] consult(String entry) {

    String[] entrys = entry.split(" "), predictions = new String[2];

    try {
      Registry myRegistryWeather = LocateRegistry.getRegistry("127.0.0.1", 9090);
      serverWeather server_weather = (serverWeather) myRegistryWeather.lookup("serverWeather");

      Registry myRegistryHoroscope = LocateRegistry.getRegistry("127.0.0.1", 9091);
      serverHoroscope server_horoscope = (serverHoroscope) myRegistryHoroscope.lookup("serverHoroscope");

      predictions[0] = server_weather.consultWeather(entrys[0]);
      predictions[1] = server_horoscope.consultHoroscope(entrys[1]);

    } catch (Exception e) {
      e.printStackTrace();
    }
    return predictions;
  }

  public static void main(String[] args) {
    try {
      Registry myRegistry = LocateRegistry.createRegistry(8080);
      myRegistry.rebind("centralServer", new centralServer());
      System.out.println("Central server ON");

    } catch (Exception e) {
      System.err.println("Error in client: " + e.getMessage());
    }
  }

}
