package SimulationOfTrams;

public class Simulate {
	private int simulationDays, currentPriority;
	private int tramCapacity, tramStopCapacity;
	private Time time;
	private Passenger[] arrayOfPassenger;
	private TramStop[] arrayOfTramStop;
	private TramLine[] arrayOfTramLine;
	private Tram[] arrayOfTram;
	private PriorityQueue eventQueue;
	
	//Info that is needed to be printed after simulation.
	private int[] timeWaiting;
	private int[] numberOfRides;
	private int waitingAfterLastRide;
	
	public Simulate(int numberOfSimulationDays) {
		this.simulationDays = numberOfSimulationDays;
		this.time = new Time(0, 6, 0);
		this.eventQueue = new PriorityQueue();
		this.numberOfRides = new int[numberOfSimulationDays];
		this.timeWaiting = new int[numberOfSimulationDays];
		this.setArraysWithAnswers();
		this.currentPriority = 0;
	}
	
	public void setArraysWithAnswers() {
		for (int i = 0; i < this.simulationDays; i++) {
			this.numberOfRides[i] = 0;
			this.timeWaiting[i] = 0;
		}
	}
	
	public void setTramStopCapacity(int tramStopCapacity) {
		this.tramStopCapacity = tramStopCapacity;
	}
	
	public void setTramCapacity(int tramCapacity) {
		this.tramCapacity = tramCapacity;
	}
	
	public void setNumberOfTramStops(int numberOfTramStops) {
		this.arrayOfTramStop = new TramStop[numberOfTramStops];
	}
	
	public void setNumberOfTramLines(int numberOfTramLines) {
		this.arrayOfTramLine = new TramLine[numberOfTramLines];
	}
	
	public void setNumberOfPassengers(int numberOfPassengers) {
		this.arrayOfPassenger = new Passenger[numberOfPassengers];
	}
	
	public void setNumberOfTrams(int numberOfTrams) {
		this.arrayOfTram = new Tram[numberOfTrams];
	}
	
	//After passenger goes in tram, it adds 1 to counter of rides in current day.
	public void nextRide() {
		this.numberOfRides[this.time.getDay()]++;
	}
	
	//Similarly to function above, it adds time passenger waited.
	public void plusTimeAwaiting(Time time) {
		this.timeWaiting[this.time.getDay()] += this.time.timeDiffInMinutes(time);
	}
	
	//Decreasing if someone went home after seeing that tram stop is full.
	public void decreaseWaitingAfterLastRide() {
		this.waitingAfterLastRide--;
	}
	
	public int getTramStopCapacity() {
		return this.tramStopCapacity;
	}
	
	public int getTramCapacity() {
		return this.tramCapacity;
	}
	
	public Time getTime() {
		return this.time;
	}
	
	public TramStop[] getTramStops() {
		return this.arrayOfTramStop;
	}
	
	public void addTramStop(int i, TramStop ts) {
		this.arrayOfTramStop[i] = ts;
	}
	
	public void addTramLine(int i, TramLine tl) {
		this.arrayOfTramLine[i] = tl;
	}
	
	public void removeEvent() {
		this.eventQueue.removeElement();
	}
	
	public Passenger createPassenger(int i) {
		Passenger p = new Passenger(this, i);
		return p;
	}
	
	public void createPassengers() {
		for (int i = 0; i < this.arrayOfPassenger.length; i++) {
			Passenger p = this.createPassenger(i);
			this.arrayOfPassenger[i] = p;
			PriorityQueueElement pqe = new PriorityQueueElement(null, p, this.currentPriority);
			this.currentPriority++;
			this.eventQueue.addElement(pqe);
		}
	}
	
	public void createTrams(int day) {
		int id = 0;
		for (int i = 0; i < this.arrayOfTramLine.length; i++) {
			TramLine tl = this.arrayOfTramLine[i];
			int numberOfTrams = tl.getNumberOfTrams();
			int minute = 0;
			int hour = 6;
			
			for (int j = 0; j < numberOfTrams; j++) {
				Time timeOfStart = new Time(day, hour, minute);
				if (j % 2 == 1) {
					Tram t = new Tram(id, tl, 2, this, timeOfStart);
					this.arrayOfTram[id] = t;
					int y = tl.getTimeLengthOfLine() / tl.getNumberOfTrams();
					this.eventQueue.addElement(new PriorityQueueElement(t, null, this.currentPriority));
					if (j > 0) {
						minute += y;
						if (minute > 60) {
							int z = minute / 60;
							minute -= z * 60;
							hour += z;
						}
					}
				}
				else {
					Tram t = new Tram(id, tl, 1, this, timeOfStart);
					this.arrayOfTram[id] = t;
					this.eventQueue.addElement(new PriorityQueueElement(t, null, this.currentPriority));
				}
				this.currentPriority++;
				id++;
			}
		}
	}
	
	public void simulateDay() {
		while (this.eventQueue.getQueueLength() != 0) {
			PriorityQueueElement nextEvent = this.eventQueue.getLastElement();
			Time firstTime = nextEvent.getTime();
			if (firstTime.compareTime(this.time) == 0) {
				nextEvent.makeEvent(this);
				this.eventQueue.sortQueue();
			}
			else {
				this.time.nextMinute();
			}
		}
	}
	
	//After simulation day this function resets needed parameters and set new for next day.
	public void clearAfterDay(int day) {
		this.eventQueue = new PriorityQueue();
		this.currentPriority = 0;
		Time time = new Time(day, 24, 0);
		for (int i = 0; i < this.arrayOfPassenger.length; i++) {
			this.arrayOfPassenger[i].newWhenTheyGo(day);
			this.arrayOfPassenger[i].setNumberInArray(-1);
			this.arrayOfPassenger[i].setWhereTheyAre(null);
			Passenger p = this.arrayOfPassenger[i];
			PriorityQueueElement pqe = new PriorityQueueElement(null, p, this.currentPriority);
			this.currentPriority++;
			this.eventQueue.addElement(pqe);
			
			//It counts time passenger waited from last going off tram to 24:00.
			if (this.arrayOfPassenger[i].getWhenTheyWentOff() != null) {
				int x = time.timeDiffInMinutes(this.arrayOfPassenger[i].getWhenTheyWentOff());
				this.timeWaiting[day - 1] += x;
			}
			this.arrayOfPassenger[i].setWhenTheyWentOff(null);
		}
		this.createTrams(day);
		for (int i = 0; i < this.arrayOfTramStop.length; i++) {
			this.arrayOfTramStop[i].resetArrayOfPassenger(this);
		}
	}
	
	public void simulation() {
		this.waitingAfterLastRide = this.simulationDays * this.arrayOfPassenger.length;
		this.createPassengers();
		this.createTrams(0);
		this.eventQueue.sortQueue();
		for (int i = 0; i < this.simulationDays; i++) {
			System.out.println("Start of day "+(i)+":");
			this.simulateDay();
			this.clearAfterDay(i + 1);
			this.eventQueue.sortQueue();
			this.time.nextDay();
		}
	}
	
	//Function used to print all needed information after simulation (with usage of System.out).
	public void returnInfo() {
		int sumOfTime = 0;
		int sumOfRides = 0;
		for (int i = 0; i < this.simulationDays; i++) {
			System.out.println("Day "+(i));
			System.out.println("Summed number of rides "+this.numberOfRides[i]);
			System.out.println("Summed time awaiting in minutes "+this.timeWaiting[i]);
			sumOfRides += this.numberOfRides[i];
			sumOfTime += this.timeWaiting[i];
		}
		System.out.println();
		System.out.println("All rides during simulation "+sumOfRides);
		System.out.print("Average time awaiting ");
		Time.averageTimeAwaiting((Double.valueOf(sumOfTime)/Double.valueOf(sumOfRides + 
								  this.waitingAfterLastRide)));
	}
}
