package SimulationOfTrams;
import java.util.Random;

public class Losowanie {
	//Function in polish because it was said to call it like that in task.
	public static int losuj(int dolna, int gorna) {
		Random number = new Random();
		int ans = number.nextInt(gorna - dolna + 1);
		return ans + dolna;
	}
}
