// $Id: Shape.java 2 2007-07-19 13:00:48Z pdezwart $

import java.awt.*;

public abstract class Shape implements java.io.Serializable, Cloneable {
	transient protected DrawingCanvas	canvas;

	protected Rectangle		bounds;
	protected boolean		isSelected;

	protected static final int	KNOB_SIZE = 6;
	protected static final int	NONE = -1;
	protected static final int	NW = 0;
	protected static final int	SW = 1;
	protected static final int	SE = 2;
	protected static final int	NE = 3;

	private	Color			color;

	/**
		Construct a Shape with a Random Color.
		@param start The Start Point of the Shape.
		@param dcanvas The DrawingCanvas to draw upon.
	*/

	public Shape(Point start, DrawingCanvas dcanvas){
		this(start, dcanvas, RandomColor.MakeColor());
	}

	/**
		Construct a Shape with a specified Color.
		@param start The Start Point of the Shape.
		@param dcanvas The DrawingCanvas to draw upon.
		@param color The Color to draw the shape as.
	*/

	public Shape(Point start, DrawingCanvas dcanvas, Color color) {
		setColor(color);
		setCanvas(dcanvas);
		bounds = new Rectangle(start);
	}

	/**
		Clones this Shape.
		@returns Object The clone()'d Shape.
	*/

	protected Object clone() throws java.lang.CloneNotSupportedException {
		Shape dolly = (Shape)super.clone();

		dolly.bounds = (Rectangle)bounds.clone();

		return(dolly);
	}

	/**
		Set the canvas handle.
		@param dcanvas The DrawingCanvas to have a handle to.
	*/

	public void setCanvas(DrawingCanvas dcanvas) {
		canvas = dcanvas;
	}

	/**
		Set the Color.
		@param color The color to be set.
	*/

	public void setColor(Color color) {
		this.color = color;
	}

	/**
		Get the Color.
		@returns Color This Shape's Color.
	*/

	public Color getColor() {
		return(color);
	}

	/**
		Abstract Draw method.
		@param g The Graphics Object to draw with.
		@param regionToDraw The region which needs to be drawn.
	*/

	public abstract void draw(Graphics g, Rectangle regionToDraw);

	/**
		Abstract method to determine if a Point lies inside this Shape.
		@param pt The Point to test.
		@returns Boolean If the Point was within this Shape.
	*/

	public abstract boolean inside(Point pt);

	/**
	* The "primitive" for all resizing/moving/creating operations that
	* affect the rect bounding box. The current implementation just resets
	* the bounds variable and triggers a re-draw of the union of the old &
	* new rectangles. This will redraw the shape in new size and place and
	* also "erase" if bounds are now smaller than before.  It is a good
	* design to have all changes to a critical variable bottleneck through
	* one method so that you can be sure that all the updating that goes
	* with it only needs to be implemented in this one place. If any of your
	* subclasses have additional work to do when the bounds change, this is
	* the method to override.  Make sure that any methods that change the
	* bounds call this method instead of directly manipulating the variable.
	*/

	protected void setBounds(Rectangle newBounds){
		Rectangle oldBounds = bounds;
		bounds = newBounds;
		updateCanvas(oldBounds.union(bounds));
	}

	/**
	* The resize operation is called when first creating a rect, as well as
	* when later resizing by dragging one of its knobs.  The two parameters
	* are the points that define the new bounding box.  The anchor point
	* is the location of the mouse-down event during a creation operation
	* or the opposite corner of the knob being dragged during a resize
	* operation. The end is the current location of the mouse.  If you
	* create the smallest rectangle which encloses these two points, you
	* will have the new bounding box.  Use the setBounds() primitive which
	* is the bottleneck we are using for all geometry changes, it handles
	* updating and redrawing.
	*/

	public void resize(Point anchor, Point end){
		Rectangle newRect = new Rectangle(anchor);
		// creates smallest rectange which
		// includes both anchor & end
		newRect.add(end);
		// reset bounds & redraw affected areas
		setBounds(newRect);
	}

	/**
	* The translate operation is called when moving a shape by dragging in
	* the canvas.  The two parameters are the delta-x and delta-y to move
	* by.  Note that either or both can be negative. Create a new rectangle
	* from our bounds and translate and then go through the setBounds()
	* primitive to change it.
	*/

	public void translate(int dx, int dy){
		Rectangle newRect = new Rectangle(bounds);
		newRect.translate(dx, dy);
		setBounds(newRect);
	}

	/**
	* Used to change the selected state of the shape which will require
	* updating the affected area of the canvas to add/remove knobs.
	*/

	public void setSelectedNoUpdate(boolean newState) {
		isSelected = newState;
	}

	public void setSelected(boolean newState){
		setSelectedNoUpdate(newState);
		// need to erase/add knobs
		// including extent of extended bounds
		updateCanvas(bounds, true);
	}

	/**
	* The updateCanvas() methods are used when the state has changed
	* in such a way that it needs to be refreshed in the canvas to properly
	* reflect the new settings. The shape should take responsibility for
	* messaging the canvas to properly update itself. The appropriate
	* AWT/JFC way to re-draw a component is to send it the repaint()
	* method with the rectangle that needs refreshing.  This will cause an
	* update()  event to be sent to the component which in turn will call
	* paint(), where the real drawing implementation goes.  See the paint()
	* method in DrawingCanvas to see how it is implemented.
	*/

	protected void updateCanvas(Rectangle areaOfChange, boolean
		enlargeForKnobs) {
		Rectangle toRedraw = new Rectangle(areaOfChange);

		if (enlargeForKnobs) {
			toRedraw.grow(KNOB_SIZE/2, KNOB_SIZE/2);
		}

		canvas.repaint(toRedraw);
	}

	protected void updateCanvas(Rectangle areaOfChange){
		updateCanvas(areaOfChange, isSelected);
	}

	public void drawKnobs(Graphics g) {
		// if selected, draw the resizing knobs along the 4 corners
		if (isSelected) { 
			Rectangle[] knobs = getKnobRects();

			for (int i = 0; i < knobs.length; i++) {
				g.fillRect(knobs[i].x, knobs[i].y,
					knobs[i].width, knobs[i].height);
			}
		}
	}

	/**
	* When needed, we create the array of knob rectangles on demand. This
	* does mean we create and discard the array and rectangles repeatedly.
	* These are small objects, so perhaps it is not a big deal, but
	* a valid alternative would be to store the array of knobs as an
	* instance variable of the Shape and and update the knobs as the bounds
	* change. This means a little more memory overhead for each Shape
	* (since it is always storing the knobs, even when not being used) and
	* having that redundant data opens up the possibility of bugs from
	* getting out of synch (bounds move but knobs didn't, etc.) but you may
	* find that a more appealing way to go.  Either way is fine with us.
	* Note this method provides a nice unified place for one override from
	* a shape subclass to substitute fewer or different knobs.
	*/

	protected Rectangle[] getKnobRects(){
		Rectangle[] knobs = new Rectangle[4];

		knobs[NW] = new Rectangle(bounds.x - KNOB_SIZE/2,
			bounds.y - KNOB_SIZE/2, KNOB_SIZE, KNOB_SIZE);

		knobs[SW] = new Rectangle(bounds.x - KNOB_SIZE/2,
			bounds.y + bounds.height - KNOB_SIZE/2, KNOB_SIZE,
			KNOB_SIZE);

		knobs[SE] = new Rectangle(bounds.x + bounds.width - KNOB_SIZE/2,
			bounds.y + bounds.height - KNOB_SIZE/2, KNOB_SIZE,
			KNOB_SIZE);

		knobs[NE] = new Rectangle(bounds.x + bounds.width - KNOB_SIZE/2,
			bounds.y - KNOB_SIZE/2, KNOB_SIZE,
			KNOB_SIZE);

		return knobs;
	}

	/**
	* Helper method to determine if a point is within one of the resize
	* corner knobs.  If not selected, we have no resize knobs, so it can't
	* have been a click on one.  Otherwise, we calculate the knob rects and
	* then check whether the point falls in one of them.  The return value
	* is one of NW, NE, SW, SE constants depending on which knob is found,
	* or NONE if the click doesn't fall within any knob.
	*/

	protected int getKnobContainingPoint(Point pt){
		// if we aren't selected, the knobs
		// aren't showing and thus there are no knobs to check
		if (!isSelected) {
			return NONE;
		}

		Rectangle[] knobs = getKnobRects();

		for (int i = 0; i < knobs.length; i++) {
			if (knobs[i].contains(pt)) {
				return i;
			}
		}

		return NONE;
	}

	/**
	* Method used by DrawingCanvas to determine if a mouse click is starting
	* a resize event. In order for it to be a resize, the click must have
	* been within one of the knob rects (checked by the helper method
	* getKnobContainingPoint) and if so, we return the "anchor" ie the knob
	* opposite this corner that will remain fixed as the user drags the
	* resizing knob of the other corner around. During the drag actions of a
	* resize, that fixed anchor point and the current mouse point will be
	* passed to the resize method, which will reset the bounds in response
	* to the movement. If the mouseLocation wasn't a click in a knob and
	* thus not the beginning of a resize event, null is returned.
	*/

	public Point getAnchorForResize(Point mouseLocation){
		int whichKnob = getKnobContainingPoint(mouseLocation);

		// no resize knob is at this location
		if (whichKnob == NONE) {
			return null;
		}

		Rectangle []	knobs = getKnobRects();

		// Ok, the code given to us in the Lab notes is borked.
		// This transformation gives the correct values.
		whichKnob = (whichKnob + (int)(knobs.length / 2)) %
			knobs.length;

		return(new Point(knobs[whichKnob].x +
			knobs[whichKnob].width / 2, knobs[whichKnob].y +
			knobs[whichKnob].height / 2));
	}
}
