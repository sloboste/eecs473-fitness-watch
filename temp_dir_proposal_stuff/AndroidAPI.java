public class BtManager{
	private BluetoothGattService currentService;

	// Constructor for init connection. Will scan and connect to appropriate
	// paired device
	public BtManager();

	// Connects to a BLE Service and sets currentService to attService
	private setService(BluetoothGattService attService);

	// returns currentService
	public BluetoothGattService getService();

	// This returns an ArrayList of all the data for a specific service
	// It will be called by other Service methods, and it is exposed to 
	// the user so that they can make there own Service methods.
	public ArrayList getCharacteristicData(BluetoothCharacteristic, int uuid);
}

public class HeartRateService
{
	// Empty Constructor because Java
	public HeartRateService();

	// Returns the pulse rate in beats per minutes
	public int getPulseRate();
}

public class GPSSevice
{
	// Empty Constructor because Java
	public GPSSevice();

	// Returns true if GPS is active, false if sleeping
	public boolean getStatus();

	// Returns current Latitude
	public int getLatitude();

	// Returns current Longitude
	public int getLongitude();
	
	// Returns the wearer's speed in kilometers per hour
	public int getSpeed();
}


public class PedometerService
{
	// Empty Constructor because Java
	public PedometerService();

	// Returns true if IMU is active, false if sleeping
	public boolean getStatus();

	//Returns the current step count
	public int getStepCount();

	//Reset the step count back to zero
	public void resetStepCount();
}

public class BatteryService
{
	// Empty Constructor because Java
	public BatteryService();

	// Returns a number between 0 and 100 representing the percent remaining
	public int getPercentage();
}
