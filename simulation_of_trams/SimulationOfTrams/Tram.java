package SimulationOfTrams;

public class Tram extends Vehicle {
	
	public Tram(int vehicleId, TramLine line, int whereItStarts, Simulate s, Time time) {
		super(vehicleId, line, whereItStarts, time, s);
				
	}
	
	//Function used to add passenger to array of passengers in tram.
	public void addPassenger(Passenger p, Simulate s) {
		s.nextRide();
		this.arrayOfPassenger[this.howManyPassenger] = p;
		p.setNumberInArray(this.howManyPassenger);
		this.howManyPassenger++;
		p.drawWhereTheyWantToGo(this);
		s.getTime().printTime();
		System.out.println("Passenger "+p.getPassengerId()+" got in Tram"+this.vehicleId
						   +" (line "+this.getLine().getTramLineId()+")"
						   +" from "+this.line.getTramStops()[this.whereItIs].getTramStopName()
						   +"(in "+p.getWhereTheyWantToGo().getTramStopName()+" direction).");
	}
	
	//Function used to remove passenger from array of passengers in tram.
	public void removePassenger(Passenger p, Simulate s) {
		int x = p.getNumberInArray();
		this.howManyPassenger--;
		this.arrayOfPassenger[p.getNumberInArray()] = this.arrayOfPassenger[this.howManyPassenger];
		this.arrayOfPassenger[this.howManyPassenger] = null;
		if (this.arrayOfPassenger[x] != null) {
			this.arrayOfPassenger[x].setNumberInArray(x);
		}
		s.getTime().printTime();
		System.out.println("Passenger "+p.getPassengerId()+" got out of Tram "+this.vehicleId+
		" (line "+this.getLine().getTramLineId()+")"+" in Tram Stop "+
		p.getWhereTheyWantToGo().getTramStopName()+".");
	}
	
	//Function used to remove all possible passengers from tram.
	public void leavePassenger(Simulate s) {
		if (this.whereItIs == 0 && this.ifEndStop == false) {
			return;
		}
		if (this.whereItIs == this.line.getLineLength() - 1 && this.ifEndStop == false) {
			return;
		}
		
		TramStop whereTramIs = this.line.getTramStops()[this.whereItIs];
		for (int i = 0; i < this.howManyPassenger; i++) {
			if (whereTramIs.getHowManyPassenger() == s.getTramStopCapacity()) {
				break;
			}
			if (this.arrayOfPassenger[i] == null) {
				break;
			}
			if (this.arrayOfPassenger[i].getWhereTheyWantToGo() == whereTramIs) {
				this.arrayOfPassenger[i].goOutTram(this, s);
				i--;
			}
		}
	}
	
	////Function used to add all possible passengers to tram.
	public void takePassenger(Simulate s) {
		if (this.whereItIs == 0 && this.ifEndStop == true) {
			return;
		}
		if (this.whereItIs == this.line.getLineLength() - 1 && this.ifEndStop == true) {
			return;
		}
		
		TramStop whereTramIs = this.line.getTramStops()[this.whereItIs];
		while (whereTramIs.getHowManyPassenger() != 0 && this.howManyPassenger < s.getTramCapacity()) {
			whereTramIs.getArrayOfPassenger()[0].goInTram(this, s);
		}
	}
	
	//Function used to move tram to next stop and sort event priority queue. 
	//If it ended its day, it is removed from queue.
	public void makeMove(Simulate s, PriorityQueueElement pqe) {
		s.getTime().printTime();
		if ((this.nextMoveTime.getHour() == 23 && this.nextMoveTime.getMinute() > 0) || 
			this.nextMoveTime.getHour() > 23) {
			if (this.whereItIs == 0 && this.whereItStarts == 1 && this.ifEndStop == true) {
				System.out.println("Tram "+this.vehicleId+" ended "+this.nextMoveTime.getDay()+" day.");
				s.removeEvent();
				return;
			}
			if (this.whereItIs == this.line.getLineLength() - 1 && this.whereItStarts == 2 && 
				this.ifEndStop == true) {
				System.out.println("Tram "+this.vehicleId+" ended "+this.nextMoveTime.getDay()+" day.");
				s.removeEvent();
				return;
			}
		}
		int[] timeBetweenStops = this.line.getTimeBetweenStops();
		if (this.whereItIs == 1 && this.whichWayItGoes == 2) {
			this.ifEndStop = true;
			this.whichWayItGoes = 1;
			this.whereItIs--;
			System.out.println("Tram "+this.vehicleId+ " arrived at "+
							   this.line.getTramStops()[0].getTramStopName()+" stop (tram terminus).");
			for (int i = 0; i < timeBetweenStops[this.line.getLineLength() - 1]; i++) {
				this.nextMoveTime.nextMinute();
			}
		}
		else if (this.whereItIs == this.line.getLineLength() - 2 && this.ifEndStop == false && 
				 this.whichWayItGoes == 1) {
			this.ifEndStop = true;
			this.whichWayItGoes = 2;
			this.whereItIs++;
			System.out.println("Tram "+this.vehicleId+" arrived at "+this.line.getTramStops()[this.whereItIs].getTramStopName()
							   +" stop (tram terminus).");
			for (int i = 0; i < timeBetweenStops[this.line.getLineLength() - 1]; i++) {
				this.nextMoveTime.nextMinute();
			}
		}
		else if (this.whereItIs == 0 && this.ifEndStop == true) {
			this.ifEndStop = false;
			System.out.println("Tram "+this.vehicleId+" arrived at "+
							   this.line.getTramStops()[0].getTramStopName()+" stop.");
			for (int i = 0; i < timeBetweenStops[0]; i++) {
				this.nextMoveTime.nextMinute();
			}
		}
		else if (this.whereItIs == this.line.getLineLength() - 1 && this.ifEndStop == true) {
			this.ifEndStop = false;
			System.out.println("Tram "+this.vehicleId+" arrived at "+
							   this.line.getTramStops()[this.whereItIs].getTramStopName()+" stop.");
			for (int i = 0; i < timeBetweenStops[this.line.getLineLength() - 2]; i++) {
				this.nextMoveTime.nextMinute();
			}
		}
		else if (this.whichWayItGoes == 1) {
			this.whereItIs++;
			System.out.println("Tram "+this.vehicleId+" arrived at "+
						       this.line.getTramStops()[this.whereItIs].getTramStopName()+" stop.");
			for (int i = 0; i < timeBetweenStops[whereItIs]; i++) {
				this.nextMoveTime.nextMinute();
			}
		}
		else {
			this.whereItIs--;
			System.out.println("Tram "+this.vehicleId+" arrived at "+
							   this.line.getTramStops()[this.whereItIs].getTramStopName()+" stop.");
			for (int i = 0; i < timeBetweenStops[whereItIs - 1]; i++) {
				this.nextMoveTime.nextMinute();
			}
		}
		
	}
	
	public String getName() {
		return "Tram";
	}
}
