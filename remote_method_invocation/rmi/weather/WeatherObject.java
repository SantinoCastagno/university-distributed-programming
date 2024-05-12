package rmi.weather;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Random;
import java.util.Properties;
import java.io.FileInputStream;

public class WeatherObject extends UnicastRemoteObject implements WeatherInterface {

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

  public WeatherObject() throws RemoteException {
    super();
  }

  @Override
  public String requestWeather(String entry) {
    int numOption = random.nextInt(options.length);

    String weather = options[numOption];

    return weather;
  }

  public static void main(String[] args) {
    Properties properties = new Properties();
    String IP = "";
    int PORT;
    try {

      properties.load(new FileInputStream("rmi/weather/config.properties"));
      PORT = Integer.parseInt(properties.getProperty("PORT"));

      Registry myRegistry = LocateRegistry.createRegistry(PORT);
      myRegistry.rebind("WeatherObject", new WeatherObject());
      System.out.println("LOG: Weather Server ON!");

    } catch (Exception e) {
      System.err.println("Error in server weather: " + e.getMessage());
    }
  }

}
