package SimulationOfTrams;

public class TramLine {
	private int tramLineId, numberOfTrams, lineLength;
	private TramStop[] tramStops;
	private int[] timeBetweenStops;
	
	public TramLine(int numberOfTrams, int lineLength, int tramLineId) {
		this.tramLineId = tramLineId;
		this.numberOfTrams = numberOfTrams;
		this.lineLength = lineLength;
		this.tramStops = new TramStop[lineLength];
		this.timeBetweenStops = new int[lineLength];
	}
	
	public void addTramStop(TramStop ts, int i, int time) {
		this.tramStops[i] = ts;
		this.timeBetweenStops[i] = time;
	}
	
	public TramStop[] getTramStops() {
		return this.tramStops;
	}
	
	public int getTramLineId() {
		return this.tramLineId;
	}
	
	public int getNumberOfTrams() {
		return this.numberOfTrams;
	}
	
	public int getLineLength() {
		return this.lineLength;
	}
	
	public int[] getTimeBetweenStops() {
		return this.timeBetweenStops;
	}
	
	public int getTimeLengthOfLine() {
		int ans = 0;
		for (int i = 0; i < this.timeBetweenStops.length; i++) {
			ans += this.timeBetweenStops[i];
		}
		ans *= 2;
		return ans;
	}
}
