package TP2;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

/**
 * client
 */
public class client {

  public static String sign = "", date = "", entry = "";

  public static void main(String[] args) {

    try {

      Registry myRegistry = LocateRegistry.getRegistry("127.0.0.1", 1234);
      server centralServer = (server) myRegistry.lookup("centralServer");

      Scanner scanner = new Scanner(System.in);

      System.out.print("Enter the sign: ");

      sign = scanner.nextLine();

      System.out.print("Enter the date: ");

      date = scanner.nextLine();

      System.out.println(sign + " " + date);

      System.out.println(centralServer.consult("increible"));

      scanner.close();

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
