package rmi.client;

import java.util.concurrent.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import rmi.central.CentralInterface;
import java.util.Random;
import java.util.Properties;
import java.io.FileInputStream;

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
    try {

      properties.load(new FileInputStream("rmi/client/config.properties"));
      IP = properties.getProperty("IP");
      PORTString = properties.getProperty("PORT");
      PORT = Integer.parseInt(PORTString);

      Registry myRegistry = LocateRegistry.getRegistry(IP, (int) PORT);
      CentralInterface centralServer = (CentralInterface) myRegistry.lookup("CentralObject");

      for (int i = 0; i < THREADS; i++) {
        Thread clienteThread = new Thread(() -> {

          String[] predictions = new String[2];
          int numOption;
          String sign, date, entry;

          try {
            numOption = random.nextInt(2);

            sign = signs[numOption];

            date = (random.nextInt(30) + 1) + "-" + (random.nextInt(12) + 1) + "-2024";

            entry = sign + " " + date;

            Thread.sleep(30);
            predictions = centralServer.requestCentral(entry);

            System.out.println("Thread: "
                + Thread.currentThread().getId() + " Client LOG: \n -> " + sign + ": " + predictions[0] + "\n -> "
                + date + ": " + predictions[1]);

          } catch (Exception e) {
            System.err.println("Client error: " + e.toString());
            e.printStackTrace();
          }
        });
        clienteThread.start();
        Thread.sleep(random.nextInt(300));
      }

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
