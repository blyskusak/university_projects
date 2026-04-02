package SimulationOfTrams;

public class TramStop {
	private String tramStopName;
	private Passenger[] arrayOfPassenger;
	private int howManyPassenger;
	
	public TramStop(String tramStopName, Simulate s) {
		this.tramStopName = tramStopName;
		this.arrayOfPassenger = new Passenger[s.getTramStopCapacity()];
		this.howManyPassenger = 0;
	}
	
	public void removePassenger() {
		this.arrayOfPassenger[0] = this.arrayOfPassenger[--this.howManyPassenger];
		this.arrayOfPassenger[this.howManyPassenger] = null;
		if (this.arrayOfPassenger[0] != null) {
			this.arrayOfPassenger[0].setNumberInArray(0);
		}
	}
	
	public void addPassenger(Passenger p) {
		this.arrayOfPassenger[this.howManyPassenger] = p;
		this.howManyPassenger++;
		p.setNumberInArray(howManyPassenger);
	}
	
	//Clears array of passenger by creating new one.
	public void resetArrayOfPassenger(Simulate s) {
		this.arrayOfPassenger = new Passenger[s.getTramStopCapacity()];
		this.howManyPassenger = 0;
	}
	
	public int getHowManyPassenger() {
		return this.howManyPassenger;
	}
	
	public String getTramStopName() {
		return this.tramStopName;
	}
	
	public Passenger[] getArrayOfPassenger() {
		return this.arrayOfPassenger;
	}
}
