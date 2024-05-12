package rmi.client;

import java.util.concurrent.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import rmi.central.CentralInterface;
import java.util.Random;
import java.util.Properties;
import java.io.FileInputStream;
import java.util.Scanner;

/**
 * Client
 */
public class ClientObject {

  private static String[] signs = { "aries", "tauro", "geminis", "cancer", "leo", "virgo", "libra", "escorpio",
      "sagitario",
      "capricornio", "acuario", "piscis" };
  private static Random random = new Random();

  private static final int THREADS = 5;

  public static void main(String[] args) {
    Properties properties = new Properties();
    String IP = "", PORTString = "";
    int PORT;
    Scanner scanner = new Scanner(System.in);
    String[] predictions = new String[2];
    int numOption;
    String sign, date, entry;

    try {
      // Sets the environment variables
      properties.load(new FileInputStream("rmi/client/config.properties"));
      IP = properties.getProperty("IP");
      PORTString = properties.getProperty("PORT");
      PORT = Integer.parseInt(PORTString);

      Registry myRegistry = LocateRegistry.getRegistry(IP, (int) PORT);
      CentralInterface centralServer = (CentralInterface) myRegistry.lookup("CentralObject");

      // Asks for the sign
      System.out.print("> Ingrese el signo: ");
      sign = scanner.nextLine();
      
      // Asks for the date
      System.out.print("> Ingrese la fecha (dd-mm-yy): ");
      date = scanner.nextLine();

      entry = sign + " " + date;

      predictions = centralServer.requestCentral(entry);

      System.out.println("Thread: "
          + Thread.currentThread().getId() + " Client LOG: \n -> " + sign + ": " + predictions[0] + "\n -> "
          + date + ": " + predictions[1]);

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
