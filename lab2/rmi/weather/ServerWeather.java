package rmi.weather;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Random;

public class ServerWeather extends UnicastRemoteObject implements WeatherInterface {

  private static String[] options = { "Sunny with clear skies and warm temperatures.",
      "Partly cloudy with a chance of scattered showers in the afternoon.",
      "Overcast with intermittent rain throughout the day.",
      "Mostly sunny with a light breeze from the northwest.",
      "Thunderstorms likely in the evening, with heavy rainfall expected.",
      "Foggy conditions in the morning, gradually clearing up by midday.",
      "A mix of sun and clouds, with mild temperatures and no precipitation.",
      "Windy conditions expected, especially in coastal areas.",
      "Freezing rain and icy roads possible, use caution when driving.",
      "Clear skies overnight with temperatures dropping below freezing." };

  private static Random random = new Random();

  public ServerWeather() throws RemoteException {
    super();
  }

  @Override
  public String consultWeather(String entry) {
    int numOption = random.nextInt(options.length);

    String weather = options[numOption];

    return weather;
  }

  public static void main(String[] args) {
    try {
      Registry myRegistry = LocateRegistry.createRegistry(9090);
      myRegistry.rebind("ServerWeather", new ServerWeather());
      System.out.println("LOG: Weather Server ON!");

    } catch (Exception e) {
      System.err.println("Error in server weather: " + e.getMessage());
    }
  }

}
