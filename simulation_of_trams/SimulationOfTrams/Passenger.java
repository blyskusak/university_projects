package SimulationOfTrams;

public class Passenger {
	private Time whenTheyGo, whenTheyWentOff;
	private int numberInArray; // -1 means that person is in home
	private int passengerId; 
	private TramStop nearTramStop, whereTheyWantToGo, whereTheyAre;
	
	public Passenger(Simulate s, int i) {
		this.numberInArray = -1;
		this.newWhenTheyGo(0);
		this.nearTramStop = this.drawNearTramStop(s.getTramStops());
		this.passengerId = i;
	}
	
	//Draws which stop is next to passenger house.
	public TramStop drawNearTramStop(TramStop[] ts) {
		return ts[Losowanie.losuj(0, ts.length - 1)];
	}
	
	//Draws where passenger wants to go.
	public void drawWhereTheyWantToGo(Tram t) {
		TramStop[] tsArray = t.getLine().getTramStops();
		int direction = t.getWhichWayItGoes();
		int whereItIs = t.getWhereItIs();
		if (direction == 1) {
			this.whereTheyWantToGo = tsArray[Losowanie.losuj(whereItIs + 1, tsArray.length - 1)];
		}
		else {
			this.whereTheyWantToGo = tsArray[Losowanie.losuj(0, whereItIs - 1)];
		}
	}
	
	//Draws time when passenger goes to tram stop.
	public void newWhenTheyGo(int day) {
		int time = Losowanie.losuj(0, 360);
		int hour = time / 60;
		this.whenTheyGo = new Time(day, hour + 6, time - hour * 60);
	}
	
	public int getNumberInArray() {
		return this.numberInArray;
	}
	
	public Time getWhenTheyGo() {
		return this.whenTheyGo;
	}
	
	public int getPassengerId() {
		return this.passengerId;
	}
	
	public TramStop getWhereTheyWantToGo() {
		return this.whereTheyWantToGo;
	}
	
	public TramStop getWhereTheyAre() {
		return this.whereTheyAre;
	}
	
	public Time getWhenTheyWentOff() {
		return this.whenTheyWentOff;
	}
	
	public void setNumberInArray(int i) {
		this.numberInArray = i;
	}
	
	public void setWhereTheyAre(TramStop ts) {
		this.whereTheyAre = ts;
	}
	
	public void setWhenTheyWentOff(Time time) {
		this.whenTheyWentOff = time;
	}
	
	//Function used to add passenger to tram and remove them from tram stop.
	public void goInTram(Tram t, Simulate s) {
		s.plusTimeAwaiting(this.whenTheyWentOff);
		this.whereTheyAre.removePassenger();
		t.addPassenger(this, s);
	}
	
	//Vice versa to previous function.
	public void goOutTram(Tram t, Simulate s) {
		if (this.whereTheyWantToGo != t.line.getTramStops()[t.whereItIs]) {
			return;
		}
		
		Time time = s.getTime();
		this.whenTheyWentOff = new Time(time.getDay(), time.getHour(), time.getMinute());
		t.removePassenger(this, s);
		this.whereTheyWantToGo.addPassenger(this);
		this.whereTheyAre = this.whereTheyWantToGo;
	}
	
	//Function used to simulate passenger going to tram stop.
	public void goOutOfHome(Simulate s) {
		if (this.nearTramStop.getHowManyPassenger() < s.getTramStopCapacity()) {
			this.nearTramStop.addPassenger(this);
			this.whereTheyAre = this.nearTramStop;
			this.whenTheyWentOff = this.whenTheyGo;
			s.getTime().printTime();
			System.out.println("Passenger"+this.passengerId+" came to Tram Stop "+
			this.nearTramStop.getTramStopName()+".");
			s.removeEvent();
			return;
		}
		s.getTime().printTime();
		s.decreaseWaitingAfterLastRide();
		System.out.println("Passenger "+this.passengerId+" came to Tram Stop "+ this.nearTramStop.getTramStopName()
						   +" but it was full so passenger returned home.");
		s.removeEvent();
	}
}
