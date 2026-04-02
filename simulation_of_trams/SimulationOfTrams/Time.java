package SimulationOfTrams;
import java.lang.Math;

public class Time {
	private int day, hour, minute;
	private String weekDay;
	
	public Time(int day, int hour, int minute) {
		this.day = day;
		this.hour = hour;
		this.minute = minute;
		this.setWeekDay();
	}
	
	//Sets week day depending on day number.
	public void setWeekDay() {
		switch (this.day % 7) {
			case 0:
				this.weekDay = "Monday";
				break;
			case 1:
				this.weekDay = "Tuesday";
				break;
			case 2:
				this.weekDay = "Wednesday";
				break;
			case 3:
				this.weekDay = "Thursday";
				break;
			case 4:
				this.weekDay = "Friday";
				break;
			case 5:
				this.weekDay = "Saturday";
				break;
			case 6:
				this.weekDay = "Sunday";
				break;
		}

	}
	
    public void nextMinute() {
    	if (this.minute == 59) {
    		this.minute = 0;
    		this.hour++;
    		return;
    	}
    	this.minute++;
    }
    
    public void nextDay() {
    	this.minute = 0;
    	this.hour = 6;
    	this.day++;
    	this.setWeekDay();
    }
    
	public int getDay() {
		return this.day;
	}
	
	public int getHour() {
		return this.hour;
	}
	
	public int getMinute() {
		return this.minute;
	}
	
	public String getWeekDay() {
		return this.weekDay;
	}
	
	//Compares two times (used to sort priority queue).
	public int compareTime(Time rhs) {
		if (rhs.getDay() > this.day) {
			return -1;
		}
		else if (rhs.getDay() == this.day) {
			if (rhs.getHour() > this.hour) {
				return -1;
			}
			else if (rhs.getHour() == this.hour) {
				if (rhs.getMinute() > this.minute) {
					return -1;
				}
				else if (rhs.getMinute() == this.minute) {
					return 0;
				}
			}
		}
		return 1;
	}
	
	public void printTime() {
		if (this.minute >= 10) {
			System.out.print("Day "+this.day+" "+this.hour+":"+this.minute+": ");
		}
		else {
			System.out.print("Day "+this.day+" "+this.hour+":0"+this.minute+": ");
		}
	}
	
	public int timeDiffInMinutes(Time time) {
		int hours, minutes;
		if (this.hour > time.getHour()) {
			hours = this.hour - time.getHour();
			minutes = this.minute - time.getMinute();
		}
		else if (this.hour < time.getHour()){
			hours = time.getHour() - this.hour;
			minutes = time.getMinute() - this.minute;
		}
		else {
			hours = 0;
			minutes = Math.abs(this.minute - time.getMinute());
		}
		
		return hours * 60 + minutes;
	}
	
	//Changes counted average time which is double to time in hh:mm:ss to print it in more readable format.
	public static void averageTimeAwaiting(double d) {
		int hours = (int) (d / 60);
		int minutes = (int) (d - hours*60);
		int seconds = (int) ((d - hours * 60 - minutes) * 60);
		System.out.print(hours+":");
		if (minutes < 10) {
			System.out.print("0"+minutes+":");
		}
		else {
			System.out.print(minutes+":");
		}
		if(seconds < 10) {
			System.out.println("0"+seconds);
		}
		else {
			System.out.println(seconds);
		}
	}
}
