// $Id: DrawingCanvas.java 2 2007-07-19 13:00:48Z pdezwart $

/*
 *-------------------------------------------------------------- 80 columns ---|
 * The DrawingCanvas class a small extension of JComponent
 * @version      1.1 15/04/01
 * @author    Julie Zelenski
 * @see       javax.swing.JComponent
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;

public class DrawingCanvas extends JComponent implements ChangeListener {
	static final int DRAG_NONE = 0;
	static final int DRAG_CREATE = 1;
	static final int DRAG_RESIZE = 2;
	static final int DRAG_MOVE = 3;

	// list of all shapes on canvas
	protected Vector allShapes;
	// currently selected shape (can be null at times)
	protected Shape selectedShape;
	// reference to toolbar to message for tool&color settings
	protected Toolbar toolbar;

	// Clipboard
	protected Clipboard clipboard;

	/**
		Cut the selected Shape from the Canvas and place it in the
		Clipboard.
	*/

	public void cut() {
		copy();
		delete();
	}

	/**
		Copy the selected Shape from the Canvas and place it in the
		Clipboard.
	*/

	public void copy() {
		clipboard.store(selectedShape);
	}
	
	/**
		Paste a Shape from the Cliboard on to the Canvas and make it
		the selected Shape.
	*/

	public void paste() {
		Shape	copy = clipboard.retrieve();

		if (copy != null) {
			// This handles the minute translation to avoid
			// overlays with consecutive paste operations.
			allShapes.add(copy);
			copy.translate(DRAG_MOVE, 0);
			setSelectedShape(copy);

			clipboard.store(copy);
			repaint();
		}
	}

	/**
		Delete the selected Shape from the Canvas.
	*/

	public void delete() {
		if (selectedShape != null) {
			allShapes.remove(selectedShape);
			selectedShape = null;
			repaint();
		}
	}

	/**
		Remove all Shapes from the Canvas.
	*/

	public void clearAll() {
		// Give the Garbage collector some work ;)
		allShapes = new Vector();

		// Make sure that there is no selectedShape
		selectedShape = null;

		repaint();
	}

	/**
		Save the Shapes on the Canvas to a file.
	*/

	public void saveToFile() {
		String			filename;
		SimpleObjectWriter	writer;
		Iterator		p = allShapes.iterator();
				
		filename = filenameChosenByUser(false);

		if (filename == null)
			return;

		writer = SimpleObjectWriter.openFileForWriting(filename); 

		if (writer == null) {
			// As per the spec, no error is reported.
			return;
		}


		while (p.hasNext()) {
			writer.writeObject(p.next());
		}

		writer.close();
	}

	/**
		Clear the Canvas and read in previously Saved Shapes in to the
		Canvas.
	*/

	public void loadFile() {
		String			filename;
		SimpleObjectReader	reader;

		Object			object;

		Vector			newShapes;
				
		filename = filenameChosenByUser(true);

		if (filename == null)
			return;

		reader = SimpleObjectReader.openFileForReading(filename); 

		if (reader == null) {
			return;
		}

		newShapes = new Vector();

		while ((object = reader.readObject()) != null) {
			newShapes.add((Shape)object);

			// Force the Shape to be unSelected as we can not
			// unSelect the Shape before saving since the Canvas'
			// contents must remain unchanged after the operation.
			((Shape)object).setSelectedNoUpdate(false);

			// Set the drawing canvas to be this one
			((Shape)object).setCanvas(this);
		}

		reader.close();

		// Only change the canvas if we got Shapes from the File
		if (newShapes.size() > 0) {
			// Replace the canvas contents
			allShapes = newShapes;

			// The canvas will need a repaint here.
			repaint();
		}
	}

	/**
		Bring the selected Shape in front of all other Shapes on the
		Canvas.
	*/

	public void bringToFront() {
		int	size = allShapes.size();

		if (selectedShape != null && size > 1) {
			int	index = allShapes.indexOf(selectedShape);
			
			if (index != size) {
				Shape	target = (Shape)allShapes.remove(index);
				allShapes.insertElementAt(target, size - 1);
				repaint();
			}
		}
	}

	/**
		Send the selected Shape behind all other Shapes on the Canvas.
	*/

	public void sendToBack() {
		int	size = allShapes.size();

		if (selectedShape != null && size > 1) {
			int	index = allShapes.indexOf(selectedShape);

			if (index != 0) {
				Shape	target = (Shape)allShapes.remove(index);
				allShapes.insertElementAt(target, 0);
				repaint();
			}
		}
	}

	/**
		Handle ChangeEvent's.
		@param e The ChangeEvent to handle.
	*/

	public void stateChanged(ChangeEvent e) {
		if (e.getSource() == toolbar) {
			if (selectedShape != null) {
				selectedShape.setColor(
					toolbar.getCurrentColor());
				repaint();
			}
		}
	}

	/**
	* Constructor for creating a new empty DrawingCanvas. We set up
	* our size and background colors, instantiate an empty vector of shapes,
	* and install a listener for mouse events using our inner class
	* CanvasMouseHandler
	*/

	public DrawingCanvas(Toolbar tb, int width, int height){
		setPreferredSize(new Dimension(width, height));
		setBackground(Color.white);
		toolbar = tb;

		// Add this DrawingCanvas as a Toolbar ChangeListener
		tb.addChangeListener(this);

		allShapes = new Vector();
		selectedShape = null;
		clipboard = new Clipboard();
		CanvasMouseHandler handler = new CanvasMouseHandler();
		addMouseListener(handler);
		addMouseMotionListener(handler);
	}

	/**
	* All components are responsible for drawing themselves in
	* response to repaint() requests.  The standard method a component
	* overrides is paint(Graphics g), but for Swing components, the default
	* paint() handler calls paintBorder(), paintComponent() and
	* paintChildren().
	*
	* For a Swing component, you override paintComponent and do your
	* drawing in that method.  For the drawing canvas, we want to
	* clear the background, then iterate through our shapes asking each
	* to draw. The Graphics object is clipped to the region to update
	* and we use to that avoid needlessly redrawing shapes outside the
	* update region.
	*/

	public void paintComponent(Graphics g){
		Rectangle regionToRedraw = g.getClipBounds();

		g.setColor(getBackground());

		g.fillRect(regionToRedraw.x, regionToRedraw.y,
			regionToRedraw.width, regionToRedraw.height);

		Iterator iter = allShapes.iterator();

		while (iter.hasNext()) {
			((Shape)iter.next()).draw(g, regionToRedraw);
		}
	}

	/**
	* Changes the currently selected shape. There is at most
	* one shape selected at a time on the canvas. It is possible
	* for the selected shape to be null. Messages the shape to
	* change its selected state which will in turn refresh the
	* shape with the knobs active.
	*/

	protected void setSelectedShape(Shape shapeToSelect) {
		// if change in selection
		if (selectedShape != shapeToSelect) {
			// deselect previous selection
			if (selectedShape != null) {
				selectedShape.setSelected(false);
			}

			// set selection to new shape
			selectedShape = shapeToSelect;

			if (selectedShape != null) {
				shapeToSelect.setSelected(true);
			}
		}
	}

	/**
	* A hit-test routine which finds the topmost shape underneath a
	* given point.We search Vector of shapes in back-to-front order
	* since shapes created later are added to end and drawn last, thus
	* appearing to be "on top" of the earlier ones.  When a click comes
	* in, we want to select the top-most shape.
	*/

	protected Shape shapeContainingPoint(Point pt){
		for (int i = allShapes.size() - 1; i >= 0; i--) {
			Shape r = (Shape)allShapes.elementAt(i);

			if (r.inside(pt)) {
				return r;
			}
		}

		return null;
	}

	/*
	* The inner class CanvasMouseHandler is the object that handles the
	* mouse actions (press, drag, release) over the canvas. Since there is
	* a bit of state to drag during the various operations (which shape,
	* where we started from, etc.) it is convenient to encapsulate all that
	* state with this little convenience object and register it as the
	* handler for mouse events on the canvas.
	*/

	protected class CanvasMouseHandler
		extends MouseAdapter implements MouseMotionListener {

		Point dragAnchor;
		// variables using to track state during drag operations
		int dragStatus;

		/** When the mouse is pressed we need to figure out what
		* action to take.  If the tool mode is arrow, the click might
		* be a select, move or reisze. If the tool mode is one of the
		* shapes, the click initiates creation of a new shape.
		*/

		public void mousePressed(MouseEvent event) {
			Shape clicked = null;
			Point curPt = event.getPoint();

			// first, determine if click was on resize knob of
			// selected shape
			if (toolbar.getCurrentTool() == Toolbar.SELECT) {
				if (selectedShape != null && (dragAnchor =
					selectedShape.getAnchorForResize(curPt))
					!= null) {
					// drag will resize this shape
					dragStatus = DRAG_RESIZE;
				} else if ((clicked =
					shapeContainingPoint(curPt)) != null) {
					// if not, check if any shape was
					// clicked
					setSelectedShape(clicked);
					// drag will move this shape
					dragStatus = DRAG_MOVE;
					dragAnchor = curPt;

					// Set the colorButton
					toolbar.setCurrentColor(
						selectedShape.getColor());
				} else {
					// else this was a click in empty area,
					// deselect selected shape,
					setSelectedShape(null);

					// drag does nothing in this case
					dragStatus = DRAG_NONE;
				}
			} else {
				Shape newShape;

				switch(toolbar.getCurrentTool()) {
					case Toolbar.RECT:
						// create rect here
						newShape = new Rect(curPt,
						DrawingCanvas.this,
						toolbar.getCurrentColor());
						break;
					case Toolbar.OVAL:
						// create oval here
						newShape = new Oval(curPt,
						DrawingCanvas.this,
						toolbar.getCurrentColor());
						break;
					case Toolbar.LINE:
						// create line here
						newShape = new Line(curPt,
						DrawingCanvas.this,
						toolbar.getCurrentColor());
						break;
					case Toolbar.SCRIBBLE:
						// create scribble here
						newShape = new Scribble(curPt,
						DrawingCanvas.this,
						toolbar.getCurrentColor());
						break;
					default:
						newShape = null;
						break;
				}
				allShapes.add(newShape);
				setSelectedShape(newShape);
				dragStatus = DRAG_CREATE;

				// drag will create (resize) this shape
				dragAnchor = curPt;
			}
		}

		/**
		* As the mouse is dragged, our listener will receive periodic
		* updates as mouseDragged events. When we get an update
		* position, we update the move/resize event that is in progress.
		*/

		public void mouseDragged(MouseEvent event){
			Point curPt = event.getPoint();

			switch (dragStatus) {
				case DRAG_MOVE:
					selectedShape.translate(curPt.x -
						dragAnchor.x, curPt.y -
						dragAnchor.y);

					// update for next dragged event
					dragAnchor = curPt;
					break;
				case DRAG_CREATE: case DRAG_RESIZE:
					selectedShape.resize(dragAnchor, curPt);
				break;
			}
		}
		public void mouseMoved(MouseEvent e) {}

	}


	/**
	* A little helper routine that will be useful for the load & save
	* operations.  It brings up the standard JFileChooser dialog and
	* allows the user to specify a file to open or save. The return
	* value is the full path to the chosen file or null if no file was
	* selected.
	*/

	protected String filenameChosenByUser(boolean forOpen){
		JFileChooser fc = new JFileChooser(
			System.getProperty("user.dir") + java.io.File.separator
			+ "Documents");

		int result = (forOpen? (fc.showOpenDialog(this)) :
			fc.showSaveDialog(this));

		java.io.File chosenFile = fc.getSelectedFile();

		if (result == JFileChooser.APPROVE_OPTION &&
			chosenFile != null) {
			return chosenFile.getPath();
		}

		return null;
		// return null if no file chosen or dialog cancelled
	}
}
