
import java.awt.*;
import java.util.*;

public class Scribble extends Shape {
	private	Vector	lines;

	public Scribble(Point start, DrawingCanvas dcanvas, Color color) {
		super(start, dcanvas, color);

		lines = new Vector();

		// Add a line with 0 length to populate the lines vector
		lines.add(new Line(start, dcanvas, color));

		bounds.add(start);
	}

	/**
		setCanvas needs to be overridded so that all the component
		Lines have their canvas set.
		@param dcanvas The DrawingCanvas to have a handle on.
	*/
	public void setCanvas(DrawingCanvas dcanvas) {
		super.setCanvas(dcanvas);

		// Handle when the Scribble is contructed
		if (lines != null) {
			Iterator	p = lines.iterator();

			while(p.hasNext()) {
				((Line)p.next()).setCanvas(dcanvas);
			}
		}
	}

	public void setColor(Color color) {
		super.setColor(color);

		// Handle the initial call from the Constructor to set the color
		if (lines == null) {
			return;
		}

		Iterator	p = lines.iterator();

		while(p.hasNext()) {
			((Line)p.next()).setColor(color);
		}
	}

	public void draw(Graphics g, Rectangle regionToDraw){
		int	size = lines.size();

		if (!bounds.intersects(regionToDraw)) {
			return;
		}

		Iterator	p = lines.iterator();

		while (p.hasNext()) {
			((Line)p.next()).draw(g, regionToDraw);
		}

		g.setColor(getColor());
		drawKnobs(g);
	}

	private Point getHead() {
		 return(((Line)lines.firstElement()).getHead());
	}

	private Point getTail() {
		return(((Line)lines.lastElement()).getTail());
	}

    /** When the DrawingCanvas needs to determine which shape is under
     * the mouse, it asks the shape to determine if a point is "inside".
     * This method should returns true if the given point is inside the
     * region for this shape. For a rectangle, any point within the
     * bounding box is inside the shape.
     */

	public void resize(Point anchor, Point end) {
		Rectangle	rect = new Rectangle(bounds);

		Point		head = getHead();
		Point		tail = getTail();

		if (anchor.equals(tail)) {
			lines.insertElementAt(new Line(end, head, canvas,
				getColor()), 0);
		} else {
			lines.add(new Line(tail, end, canvas, getColor()));
		}

		rect.add(end);

		// Don't call the super class resize as this will bork the
		// bounding box.
		//
		// Instead, call setBounds directly with the new bounding
		// box.
		setBounds(rect);
	}

	public void translate(int dx, int dy) {
		Iterator	p = lines.iterator();

		while(p.hasNext()) {
			((Line)p.next()).translate(dx, dy);
		}

		super.translate(dx, dy);
	}
	
	public boolean inside(Point pt) {
		boolean	in = false;

		if (!bounds.contains(pt)) {
			return(false);
		}

		Iterator	p = lines.iterator();

		while (!in && p.hasNext()) {
			in = ((Line)p.next()).inside(pt);
		}

		return(in);
	}

	protected Rectangle[] getKnobRects() {
		Rectangle[]	knobs = new Rectangle[2];

		Point	head = getHead();
		Point	tail = getTail();

		knobs[0] = new Rectangle(head.x - KNOB_SIZE/2,
				head.y - KNOB_SIZE/2, KNOB_SIZE, KNOB_SIZE);
		knobs[1] = new Rectangle(tail.x - KNOB_SIZE/2,
				tail.y - KNOB_SIZE/2, KNOB_SIZE, KNOB_SIZE);

		return(knobs);
	}

	protected Object clone() throws java.lang.CloneNotSupportedException {
		Scribble	dolly = (Scribble)super.clone();
		Iterator	p = lines.iterator();

		dolly.lines = new Vector(lines.size());

		while (p.hasNext()) {
			dolly.lines.add(((Line)p.next()).clone());
		}

		return(dolly);
	}
}
