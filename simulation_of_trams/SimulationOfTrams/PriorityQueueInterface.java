package SimulationOfTrams;

public interface PriorityQueueInterface {
	public void addElement(PriorityQueueElement pqe);
	public void removeElement();
	public PriorityQueueElement getLastElement();
	public void resizeQueue();
	public int getQueueLength();
}
