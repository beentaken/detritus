/**
 *-------------------------------------------------------------- 80 columns ---|
 * The Toolbar class sets up the buttons along the bottom of the window
 * that allow the user to choose the current drawing tool as well as the
 * the button that shows the current color and brings up the color chooser
 * to pick a new color.  The code that's here should do pretty much all
 * you need as is. Read it over to get a sense of what it takes to put
 * together and manage some simple UI. You are welcome to futz with this
 * code if you want to make some changes, but it shouldn't be necessary.
 *
 * @version      1.1 15/04/01
 * @author       Julie Zelenski
 * @see          javax.swing.JPanel
 * @see          javax.swing.JToggleButton
 * @see          javax.swing.JButton
 */

import java.util.Vector;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import javax.swing.event.*;

public class Toolbar extends JPanel{
    public static final int SELECT = 0;
    public static final int RECT = 1;
    public static final int OVAL = 2;
    public static final int LINE = 3;
    public static final int SCRIBBLE = 4;

    protected JToggleButton[] toolButtons;
    protected JButton colorButton;
    protected Vector listeners;

    protected static final String[]
	toolNames = {"Select", "Rect", "Oval", "Line", "Scribble"};

    /**
     * Returns current tool selected, using the public enumerated Toolbar
     * constants SELECT, RECT, OVAL, etc. to identify the different tools.
     * If there is currently no selected button, (shouldn't happen), -1
     * is returned.
     */

    public int getCurrentTool(){
	for (int i = 0; i < toolButtons.length; i++)
	    if (toolButtons[i].isSelected()) return i;
	return -1;
    }

    /**
     * Changes current tool, by selecting a new button and deselecting
     * previous selection. toolNum parameter should be one of the
     * public enumerated Toolbar constants SELECT, RECT, OVAL, etc.
     * If invalid, an array out of bounds exception will be thrown.
     */

    public void setCurrentTool(int toolNum){
	toolButtons[toolNum].setSelected(true);
    }

    /**
     * Returns current color showing on the toolbar's color button.
     */

    public Color getCurrentColor(){
	return colorButton.getBackground();
    }

    /**
     * Sets current color showing on the toolbar's color button.
     * Anytime the current color is changed, a ChangeEvent is
     * sent to all registered listeners.
     */

    public void setCurrentColor(Color color){
	colorButton.setBackground(color);
	fireStateChanged();
    }

    /**
     * Adds a ChangeListener to the toolbar. Whenever the
     * color is changed on the toolbar, the listener will be
     * notified with a change event.
     */

    public void addChangeListener(ChangeListener l){
        listeners.addElement(l);
    }

    /**
     * Removes a ChangeListener from the toolbar.
     */

    public void removeChangeListener(ChangeListener l){
        listeners.removeElement(l);
    }

    /*
     * Notify all listeners registered for notification when the
     * toolbar change.
     */

    protected void fireStateChanged(){
        ChangeEvent changeEvent = new ChangeEvent(this);
	for (int i = 0; i < listeners.size(); i++)
	    ((ChangeListener)listeners.elementAt(i)).stateChanged(changeEvent);
    }

    /**
     * Constructor for the Toolbar. Creates the tool buttons, arranges
     * within the panel in proper layout, attachs necessary listeners, etc.
     * Records references to the needed buttons so we can later set/get the
     * tool and color choices.
     */

    public Toolbar(){
	setLayout(new FlowLayout(FlowLayout.CENTER, 0, 3));
	Insets noMargin = new Insets(0,0,0,0);
	// Create a row of toggle buttons, each with a tool image, put into a button group so
	// turning on one turns off the others.
	ButtonGroup group = new ButtonGroup();
	toolButtons = new JToggleButton[toolNames.length];
	for (int i = 0; i < toolNames.length; i++) {
	    toolButtons[i] = new JToggleButton(new ImageIcon("Images" +
							     java.io.File.separator +
							     toolNames[i] +
							     ".GIF"),
					       i==0);
	    group.add(toolButtons[i]);
	    add(toolButtons[i]);
	    toolButtons[i].setMargin(noMargin);
	}

	add(new JLabel("   Fill color "));
	colorButton = new JButton(new ImageIcon("Images" + java.io.File.separator + "Bucket.GIF"));
	colorButton.setBackground(Color.darkGray);
	colorButton.setMargin(noMargin);
	add(colorButton);
	colorButton.addActionListener( new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    Color chosenColor;
		    if ((chosenColor =  JColorChooser.showDialog(null, "Choose shape fill color", getCurrentColor())) != null)
			setCurrentColor(chosenColor);
		}});
	listeners = new Vector();
    }

}


