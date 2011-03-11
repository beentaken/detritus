import java.awt.*;
import java.awt.geom.*;

/**
	A circle that can be manipulated and that draws itself on a canvas.
	@version $Revision: 1.11 $
	@author $Author: dezwart $
*/
public class Circle extends BasicShape {
	public Circle() {
		this(0);
	}

	public Circle(double radius) {
		this(0, 0, "black", radius);
	}

	public Circle(double x, double y, String c, double radius) {
		super(x, y, c);
		setRadius(radius);
	}

	public void setRadius(double radius) {
		this.radius = radius;
	}

	public void draw() {
		double	diameter = 2 * radius;

		super.draw(new Ellipse2D.Double(X(), Y(), diameter, diameter));
	}

	private double radius;
}
