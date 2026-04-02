package SimulationOfTrams;
import java.util.Scanner;


public class Main {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);

		String tmp = sc.next();
		int numberOfSimulationDays = Integer.parseInt(tmp);
		Simulate thisSimulation = new Simulate(numberOfSimulationDays);
		System.out.println(numberOfSimulationDays);


		for (int i = 1; i < 6; i++) {
			tmp = sc.next();

			if (i == 1) {
				int tramStopCapacity = Integer.parseInt(tmp);
				thisSimulation.setTramStopCapacity(tramStopCapacity);
				System.out.println(tramStopCapacity);
			}

			else if (i == 2) {
				int numberOfTramStops = Integer.parseInt(tmp);
				thisSimulation.setNumberOfTramStops(numberOfTramStops);
				System.out.println(numberOfTramStops);
				for (int j = 0; j < numberOfTramStops; j++) {
					String tsName = sc.next();
					TramStop ts = new TramStop(tsName, thisSimulation);
					thisSimulation.addTramStop(j, ts);
					System.out.print(tsName+" ");
				}
			}

			else if (i == 3) {
				int numberOfPassengers = Integer.parseInt(tmp);
				thisSimulation.setNumberOfPassengers(numberOfPassengers);
				System.out.println();
				System.out.println(numberOfPassengers);
			}
			
			else if (i == 4) {
				int tramCapacity = Integer.parseInt(tmp);
				thisSimulation.setTramCapacity(tramCapacity);
				System.out.println(tramCapacity);
			}
	
			else if (i == 5) {
				int numberOfTramLines = Integer.parseInt(tmp);
				thisSimulation.setNumberOfTramLines(numberOfTramLines);
				int numberOfAllTrams = 0;
				System.out.println(numberOfTramLines);
				for (int j = 0; j < numberOfTramLines; j++) {
					int numberOfTrams = Integer.parseInt(sc.next());
					numberOfAllTrams += numberOfTrams;
					int lineLength = Integer.parseInt(sc.next());
					
					TramLine tl = new TramLine(numberOfTrams, lineLength, j);
					System.out.print(numberOfTrams+" "+lineLength+" ");
	
					for (int k = 0; k < lineLength; k++) {
						String tramStopName = sc.next();
						int timeBetweenStops = Integer.parseInt(sc.next());
						
						System.out.println(tramStopName+" "+timeBetweenStops+" ");
						
						for (int l = 0; l < thisSimulation.getTramStops().length; l++) {
							if (thisSimulation.getTramStops()[l].getTramStopName().equals(tramStopName)) {
								tl.addTramStop(thisSimulation.getTramStops()[l], k, timeBetweenStops);
								break;
							}
						}
						thisSimulation.addTramLine(j, tl);
					}
				}
				thisSimulation.setNumberOfTrams(numberOfAllTrams);
			}
		}
		sc.close();
                
		thisSimulation.simulation();
		System.out.println();
		thisSimulation.returnInfo();
	    }
}