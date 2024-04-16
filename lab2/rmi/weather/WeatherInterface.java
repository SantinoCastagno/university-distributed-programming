package rmi.weather;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface WeatherInterface extends Remote {
  public String requestWeather(String entry) throws RemoteException;

}
