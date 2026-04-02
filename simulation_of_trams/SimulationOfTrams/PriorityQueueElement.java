package SimulationOfTrams;

public class PriorityQueueElement {
	private Tram eventTram;
	private Passenger eventPassenger;
	private int tramOrPassenger; //Passenger has 2, tram coming - 1.
	private int priority; //The higher priority, the later it was put in queue.
	
	public PriorityQueueElement(Tram eventTram, Passenger eventPassenger, int priority) {
		this.eventTram = eventTram;
		this.eventPassenger = eventPassenger;
		if (eventTram == null) {
			this.tramOrPassenger = 2;
		}
		else {
			this.tramOrPassenger = 1;
		}
		this.priority = priority;
	}
	
	public int getTramOrPassenger() {
		return this.tramOrPassenger;
	}
	
	public int getPriority() {
		return this.priority;
	}
	
	public Tram getEventTram() {
		return this.eventTram;
	}
	
	public Passenger getEventPassenger() {
		return this.eventPassenger;
	}
	
	public Time getTime() {
		if (this.eventTram == null) {
			return this.eventPassenger.getWhenTheyGo();
		}
		return this.eventTram.getNextMoveTime();
	}
	
	//Function simulates event (in both passenger and tram case).
	public void makeEvent(Simulate s) {
		if (this.eventTram == null) {
			this.eventPassenger.goOutOfHome(s);
		}
		else {
			this.eventTram.makeMove(s, this);
			this.eventTram.leavePassenger(s);
			this.eventTram.takePassenger(s);
		}
	}
}
