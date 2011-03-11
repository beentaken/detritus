import java.awt.*;
import java.awt.geom.*;

/**
	A Box
	@version $Revision: 1.15 $
	@author $Author: dezwart $
*/
public class Box extends BasicShape {
	public Box() {
		this(0, 0);
	}

	public Box(double width, double height) {
		this(0, 0, "black", width, height);
	}

	public Box(double x, double y, String c, double width, double height) {
		super(x, y, c);
		setDimensions(width, height);
	}

	public void setDimensions(double width, double height) {
		this.width = width;
		this.height = height;
	}

	public void draw() {
		super.draw(new Rectangle2D.Double(X(), Y(), width, height));
	}

	private double	width;
	private double	height;
}
