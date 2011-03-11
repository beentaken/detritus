/**
	A cartesian point (co-ordinate)
	@version $Revision: 1.9 $
	@author $Author: dezwart $
*/
public abstract class Point {
	public Point() {
		this(0, 0);
	}

	public Point(double x, double y) {
		set(x, y);
	}

	/**
		Sets the point to a particular spot.
	*/
	public void set(double x, double y) {
		this.x = x;
		this.y = y;
	}

	/**
		Moves the point from where it is by x & y.
	*/
	public void move(double x, double y) {
		set(this.x + x, this.y + y);
	}

	public double X() {
		return(x);
	}

	public double Y() {
		return(y);
	}

	private double	x;
	private double	y;
}
