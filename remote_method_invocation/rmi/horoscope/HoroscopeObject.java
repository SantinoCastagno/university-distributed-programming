package rmi.horoscope;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Random;
import java.util.Properties;
import java.io.FileInputStream;

public class HoroscopeObject extends UnicastRemoteObject implements HoroscopeInterface {

  private static String[] options = { "You will embark on an exciting adventure next year.",
      "A new opportunity will present itself to you soon.",
      "Your hard work will pay off in unexpected ways.",
      "A significant change is on the horizon for you.",
      "Someone from your past will re-enter your life.",
      "You will make a valuable connection that will change your path.",
      "Travel will be in your future, leading to new experiences.",
      "Your creativity will flourish, leading to exciting projects.",
      "An unexpected gift will bring joy to your life.",
      "You will find inner peace through self-d" };

  private static Random random = new Random();

  public HoroscopeObject() throws RemoteException {
    super();
  }

  @Override
  public String requestHoroscope(String entry) {
    int numOption = random.nextInt(options.length);

    String horoscope = options[numOption];

    return horoscope;
  }

  public static void main(String[] args) {
    Properties properties = new Properties();
    String IP = "";
    int PORT;

    try {

      properties.load(new FileInputStream("rmi/horoscope/config.properties"));
      IP = properties.getProperty("IP");
      PORT = Integer.parseInt(properties.getProperty("PORT"));

      Registry myRegistry = LocateRegistry.createRegistry(PORT);
      myRegistry.rebind("HoroscopeObject", new HoroscopeObject());
      System.out.println("LOG: Horoscope server ON!");

    } catch (Exception e) {
      System.err.println("Error in horoscope: " + e.getMessage());
    }
  }

}
