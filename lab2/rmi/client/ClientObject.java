package rmi.client;

import java.util.concurrent.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import rmi.central.CentralInterface;
import java.util.Random;

/**
 * Client
 */
public class ClientObject {

  // private static String entry = "";
  private static String[] signs = { "aries", "tauro", "geminis", "cancer", "leo", "virgo", "libra", "escorpio",
      "sagitario",
      "capricornio", "acuario", "piscis" };
  private static Random random = new Random();
  // private static String[] predictions = new String[2];

  public static void main(String[] args) {
    try {

      Registry myRegistry = LocateRegistry.getRegistry("127.0.0.1", 8080);
      CentralInterface centralServer = (CentralInterface) myRegistry.lookup("CentralObject");

      // Scanner scanner = new Scanner(System.in);

      // System.out.print("Enter the sign and date: ");

      // entry = scanner.nextLine();
      for (int i = 0; i < 5; i++) {
        Thread clienteThread = new Thread(() -> {

          String[] predictions = new String[2];
          int numOption;
          String sign, date, entry;

          try {

            System.out.println("Thread: " + Thread.currentThread().getId() + " consulting");

            numOption = random.nextInt(signs.length);

            sign = signs[numOption];

            date = (random.nextInt(30) + 1) + "-" + (random.nextInt(12) + 1) + "-2024";

            System.out.println("Thread: " + Thread.currentThread().getId() + " sign: " + sign + " date: " + date);

            entry = sign + " " + date;

            predictions = centralServer.requestCentral(entry);

            System.out.println("Thread: "
                + Thread.currentThread().getId() + " Client LOG: \n -> " + predictions[0] + "\n -> " + predictions[1]);

          } catch (Exception e) {
            System.err.println("Client error: " + e.toString());
            e.printStackTrace();
          }
        });
        clienteThread.start();
      }
      // predictions = centralServe.requestCentral("horosocpo clima");

      // System.out.println("Client LOG: \n -> " + predictions[0] + "\n -> " +
      // predictions[1]);

      // scanner.close();

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
