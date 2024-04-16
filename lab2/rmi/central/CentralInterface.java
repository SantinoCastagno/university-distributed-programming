package rmi.central;
import java.rmi.*;

public interface CentralInterface extends Remote{
	public String[] requestCentral(String entry) throws RemoteException;
}

