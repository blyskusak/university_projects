package SimulationOfTrams;

public abstract class Vehicle {
	protected int vehicleId;
	protected int whereItStarts, whichWayItGoes; // 1 - begin of line, 2 - end of line.
	protected int whereItIs; // Number in array of line stops.
	protected TramLine line;
	protected Passenger[] arrayOfPassenger;
	protected int howManyPassenger;
	protected Time nextMoveTime;
	protected boolean ifEndStop;
	
	public Vehicle(int vehicleId, TramLine line, int whereItStarts, Time time, Simulate s) {
		this.vehicleId = vehicleId;
		this.line = line;
		this.whereItStarts = whereItStarts;
		this.whichWayItGoes = whereItStarts;
		if (whereItStarts == 1) {
			this.whereItIs = 0;
		}
		else {
			this.whereItIs = line.getTramStops().length - 1;
		}
		this.arrayOfPassenger = new Passenger[s.getTramCapacity()];
		this.nextMoveTime = time;
		this.ifEndStop = true;
		this.howManyPassenger = 0;
	}
	
	public int getVehicleId() {
		return this.vehicleId;
	}
	
	public int getWhereItStarts() {
		return this.whereItStarts;
	}
	
	public int getWhereItIs() {
		return this.whereItIs;
	}
	
	public int getWhichWayItGoes() {
		return this.whichWayItGoes;
	}
	
	public TramLine getLine() {
		return this.line;
	}
	
	public int getHowManyPassenger() {
		return this.howManyPassenger;
	}
	
	public Time getNextMoveTime() {
		return this.nextMoveTime;
	}
}
