// $Id: Line.java 2 2007-07-19 13:00:48Z pdezwart $

import java.awt.*;

public class Line extends Shape {
	public static final int	hitTolerance = 6;

	protected Point		head;
	protected Point		tail;

	public Line(Point start, DrawingCanvas dcanvas, Color color) {
		this(start, start, dcanvas, color);
	}

	public Line(Point start, Point end, DrawingCanvas dc, Color color) {
		super(start, dc, color);

		// Clones of the points have to be made to avoid problems in
		// the Squggle as each point is part of two lines.
		resize((Point)start.clone(), (Point)end.clone());
	}

	public void draw(Graphics g, Rectangle regionToDraw){
		if (!bounds.intersects(regionToDraw))
			return;

		g.setColor(getColor());
		g.drawLine(head.x, head.y, tail.x, tail.y);
		drawKnobs(g);
	}

	public Point getHead() {
		return(head);
	}

	public Point getTail() {
		return(tail);
	}

	public void resize(Point anchor, Point end) {
		head = anchor;
		tail = end;

		super.resize(anchor, end);

		// Fix the feature with some Java VM's when one of the Bounding
		// rectangle's dimensions is 0.

		if(head.x == tail.x) {
			bounds.width++;
		}

		if (head.y == tail.y) {
			bounds.height++;
		}
	}

	public void translate(int dx, int dy) {
		head.translate(dx, dy);
		tail.translate(dx, dy);
		super.translate(dx, dy);
	}
	
	public boolean inside(Point pt) {
		Rectangle	bigbounds = (Rectangle)bounds.clone();

		bigbounds.grow(hitTolerance, hitTolerance);

		// Check to see that we are within the bounding box of the
		// line with some tolerance
		if (!bigbounds.contains(pt))
			return(false);

		double	width = tail.getX() - head.getX();
		double	height = tail.getY() - head.getY();
		double	dx = pt.getX() - head.getX();
		double	dy = pt.getY() - head.getY();

		double	distance, angleToCorner, angleToPoint;

		if (dx != 0 && width != 0) {
			angleToCorner = Math.atan(height/width);
			angleToPoint = Math.atan(dy/dx);
			distance = Math.sqrt(dx * dx + dy * dy) *
				Math.sin(
					Math.abs(angleToCorner - angleToPoint));
		} else {
			distance = Math.abs(dx);
		}

		return(distance < (double)hitTolerance);
	}


	protected Rectangle[] getKnobRects() {
		Rectangle[]	knobs = new Rectangle[2];

		knobs[0] = new Rectangle(head.x - KNOB_SIZE/2,
				head.y - KNOB_SIZE/2, KNOB_SIZE, KNOB_SIZE);
		knobs[1] = new Rectangle(tail.x - KNOB_SIZE/2,
				tail.y - KNOB_SIZE/2, KNOB_SIZE, KNOB_SIZE);

		return(knobs);
	}

	protected Object clone() throws java.lang.CloneNotSupportedException {
		Line	dolly = (Line)super.clone();
		dolly.head = (Point)head.clone();
		dolly.tail = (Point)tail.clone();

		return(dolly);
	}
}
