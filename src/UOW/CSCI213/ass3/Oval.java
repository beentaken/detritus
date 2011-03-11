// $Id: Oval.java 2 2007-07-19 13:00:48Z pdezwart $

import java.awt.*;

public class Oval extends Shape {
	public Oval(Point start, DrawingCanvas dcanvas, Color color) {
		super(start, dcanvas, color);
	}

	public void draw(Graphics g, Rectangle regionToDraw){
		if (!bounds.intersects(regionToDraw)) {
			return;
		}

		g.setColor(getColor());
		g.fillOval(bounds.x, bounds.y, bounds.width, bounds.height);
		drawKnobs(g);
	}

	public boolean inside(Point pt){
		double	halfWidth = bounds.width / 2.0;
		double	halfHeight = bounds.height / 2.0;

		// Sanity Check
		if (halfWidth == 0.0 || halfHeight == 0.0) {
			return(false);
		}

		double	x = bounds.x + halfWidth;
		double	y = bounds.y + halfHeight;

		double	pX = pt.getX();
		double	pY = pt.getY();

		double normx = (pX - x) / halfWidth;
		double normy = (pY - y) / halfHeight;

		return((normx * normx + normy * normy) <= 1.0);
	}
}
