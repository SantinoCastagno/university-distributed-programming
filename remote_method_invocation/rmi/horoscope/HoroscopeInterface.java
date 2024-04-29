package rmi.horoscope;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface HoroscopeInterface extends Remote {
  public String requestHoroscope(String entry) throws RemoteException;
}
