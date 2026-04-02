package SimulationOfTrams;

import java.util.Arrays;
import java.util.Comparator;

public class PriorityQueue implements PriorityQueueInterface{
	private PriorityQueueElement[] pq;
	private int queueLength;
	
	public PriorityQueue() {
		this.pq = new PriorityQueueElement[10];
		this.queueLength = 0;
	}
	
	public void addElement(PriorityQueueElement pqe) {
		if (this.queueLength == this.pq.length) {
			this.resizeQueue();
		}
		this.pq[this.queueLength] = pqe;
		this.queueLength++;
	}
	
	public void sortQueue() {
		Arrays.sort(this.pq, new Comparator<PriorityQueueElement>() {
			@Override
			public int compare(PriorityQueueElement lhs, PriorityQueueElement rhs) {
				if (rhs == null) {
					return -1;
				}
				if (lhs == null) {
					return 1;
				}
				
				Time lhsTime, rhsTime;
				if (lhs.getTramOrPassenger() == 2) {
					lhsTime = lhs.getEventPassenger().getWhenTheyGo();
				}
				else {
					lhsTime = lhs.getEventTram().getNextMoveTime();
				}
				if (rhs.getTramOrPassenger() == 2) {
					rhsTime = rhs.getEventPassenger().getWhenTheyGo();
				}
				else {
					rhsTime = rhs.getEventTram().getNextMoveTime();
				}
				int timeCompared = lhsTime.compareTime(rhsTime);
				// We want to keep events sorted in descending order because after deleting
				// event, it will not cause "hole" at the beginning of the queue.
				if (timeCompared == 1) {
					return -1;
				}
				else if (timeCompared == -1) {
					return 1;
				}
				else {
					if (lhs.getPriority() > rhs.getPriority()) {
						return 1;
					}
					else if (lhs.getPriority() < rhs.getPriority()) {
						return -1;
					}
				}
				return 0;
			}
		});
	}
	
	public void removeElement() {
		assert this.queueLength > 0 : "Queue is empty";
		this.pq[this.queueLength - 1] = null;
		this.queueLength--;
	}
	
	public PriorityQueueElement getLastElement() {
		assert this.queueLength > 0 : "Queue is empty";
		return this.pq[this.queueLength - 1];
	}
	
	public void resizeQueue() {
		PriorityQueueElement[] newQueue = new PriorityQueueElement[2 * this.pq.length];
		for (int i = 0; i < this.pq.length; i++) {
			newQueue[i] = this.pq[i];
		}
		this.pq = newQueue;
	}
	
	public int getQueueLength() {
		return this.queueLength;
	}
}
