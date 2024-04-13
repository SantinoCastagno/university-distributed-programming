package rmi;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

/**
 * client
 */
public class client {

  public static String entry = "";

  public static void main(String[] args) {
    String[] predictions = new String[2];

    try {

      Registry myRegistry = LocateRegistry.getRegistry("127.0.0.1", 8080);
      server centralServer = (server) myRegistry.lookup("centralServer");

      Scanner scanner = new Scanner(System.in);

      System.out.print("Enter the sign and date: ");

      entry = scanner.nextLine();

      predictions = centralServer.consult(entry);

      System.out.println(predictions[0] + "\n" + predictions[1]);

      scanner.close();

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
