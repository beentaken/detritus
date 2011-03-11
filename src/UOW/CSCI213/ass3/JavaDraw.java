/**
 *-------------------------------------------------------------- 80 columns ---|
 * This is the main class for the application. It is built along the same
 * lines as the Editor class of project 1. It has a constructor, two instance
 * methods, and a static main()  that kicks the whole thing off.
 * The two instance methods are a menu creation method, and a menuItems
 * initialisation method.  The constructor instantiates the window
 * and sets up its internals by creating and installing the drawing canvas,
 * toolbar, and menus. All of the code works correctly as is and should
 * require no changes.
 *
 * @version      1.1 15/04/01
 * @author       Julie Zelenski
 * @see       	 javax.swing.JMenuBar
 * @see       	 javax.swing.JMenuItem
 * @see       	 javax.swing.JMenu
 */

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

public class JavaDraw extends JFrame {
    final Toolbar toolbar = new Toolbar();
    final DrawingCanvas canvas = new DrawingCanvas(toolbar, 350, 350);
    final int menuMask = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
    final JMenuBar mb = new JMenuBar();
    final String[]
	    fileMenuItems = {"Clear all", "Load file", "Save to file", "Quit"};
    final int[] fileKeyCodes = {KeyEvent.VK_N, KeyEvent.VK_O, KeyEvent.VK_S, KeyEvent.VK_Q};
    final ActionListener[] fileActionListeners = {
	new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.clearAll();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.loadFile();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.saveToFile();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    System.exit(0);}}
    };
    final String[] editMenuItems = {"Cut", "Copy", "Paste", "Delete"};
    final int[] editKeyCodes = {KeyEvent.VK_X, KeyEvent.VK_C, KeyEvent.VK_V, KeyEvent.VK_BACK_SPACE};
    final int[] editMenuMasks = {menuMask, menuMask, menuMask, 0};
    final ActionListener[] editActionListeners = {
	new ActionListener() {
		public void actionPerformed(ActionEvent e) { canvas.cut();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) { canvas.copy();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) { canvas.paste();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) { canvas.delete();}}
    };
    final String[] layeringMenuItems = {"Bring to front", "Send to back"};
    final int[]    layeringKeyCodes = {KeyEvent.VK_F, KeyEvent.VK_B};
    final ActionListener[] layeringActionListeners = {
	new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.bringToFront();}},
	new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    canvas.sendToBack();}}
    };

    private JavaDraw(){
	super("JavaDraw!");
	getContentPane().add(toolbar, BorderLayout.SOUTH);
	getContentPane().add(canvas, BorderLayout.CENTER);
	createMenus();
	setJMenuBar(mb);
	setLocation(100, 20);
	pack();
	setVisible(true);
    }


    static public void main(String[] args){
	JavaDraw javaDraw = new JavaDraw();
    }

    private void initMenus(JMenu m,
			   String  miLabel,
			   int keyCode,
			   int menuMask,
			   ActionListener al){
	JMenuItem mi = new JMenuItem(miLabel);
	m.add(mi);
	mi.addActionListener(al);
	mi.setAccelerator(KeyStroke.getKeyStroke(keyCode, menuMask));
    }

    private void createMenus(){
	JMenu m;
	m = new JMenu("File");
	for(int i = 0; i < fileMenuItems.length; i++)
	    initMenus(m,
		      fileMenuItems[i],
		      fileKeyCodes[i],
		      menuMask,
		      fileActionListeners[i]);
	mb.add(m);
	m = new JMenu("Edit");
	for(int i = 0; i < editMenuItems.length; i++)
	    initMenus(m,
		      editMenuItems[i],
		      editKeyCodes[i],
		      editMenuMasks[i],
		      editActionListeners[i]);
	mb.add(m);
	m = new JMenu("Layering");
	for(int i = 0; i < layeringMenuItems.length; i++)
	    initMenus(m,
		      layeringMenuItems[i],
		      layeringKeyCodes[i],
		      menuMask,
		      layeringActionListeners[i]);
	mb.add(m);
    }
}





