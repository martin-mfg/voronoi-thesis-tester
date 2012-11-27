import icons.Icons;

import java.awt.AWTException;
import java.awt.BorderLayout;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Robot;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.SwingUtilities;
import javax.swing.border.BevelBorder;

public class JMalPanel extends JPanel {

	private static enum State {
		Add, Move, Remove;
	}

	private static final int GRID_SIZE = 20;

	private static final int CATCH_RADIUS = 10;

	private Map<Paintable.PaintableType, List<Paintable>> paintables = new HashMap<Paintable.PaintableType, List<Paintable>>();

	private Map<CoordinatePair, Set<Catchable>> gridMap = new HashMap<CoordinatePair, Set<Catchable>>();

	private JToolBar toolBar = new JMyToolBar();

	private JComponent drawingArea;

	private Catchable currentCaughtCatchable = null;

	private Catchable catchableDragged = null;

	private State state = null;

	private Robot robot;

	private MouseAdapter mouseAdapter = new MyMouseListener();

	private Dot.DotColor currentDotColor = Dot.DotColor.RED;

	public JMalPanel() {
		super(new BorderLayout());
		initDrawingArea();
		add(drawingArea, BorderLayout.CENTER);
		add(toolBar, BorderLayout.NORTH);
		setOpaque(false);
		try {
			robot = new Robot();
		} catch (AWTException e) {
			e.printStackTrace();
		}
	}

	private void initDrawingArea() {
		drawingArea = new JPanel() {

			@Override
			protected void paintComponent(Graphics g) {
				Graphics2D sg = (Graphics2D) g;
				if (paintables.containsKey(Paintable.PaintableType.Point)) {
					for (Paintable paintable : paintables
							.get(Paintable.PaintableType.Point)) {
						paintable.paintObject(sg);
					}
				}
				if (paintables.containsKey(Paintable.PaintableType.Edge)) {
					for (Paintable paintable : paintables
							.get(Paintable.PaintableType.Edge)) {
						paintable.paintObject(sg);
					}
				}
			}
		};
		drawingArea.setBorder(BorderFactory
				.createBevelBorder(BevelBorder.LOWERED));
		drawingArea.addMouseListener(mouseAdapter);
	}

	public void addPaintable(Paintable newPaintable) {
		if (newPaintable instanceof Catchable) {
			Catchable newCatchable = (Catchable) newPaintable;
			for (CoordinatePair cp : newCatchable.getCatchPoints()) {
				CoordinatePair gridCell = new CoordinatePair(cp.getX()
						/ GRID_SIZE, cp.getY() / GRID_SIZE);

				if (!gridMap.containsKey(gridCell)) {
					gridMap.put(gridCell, new HashSet<Catchable>());
				}
				gridMap.get(gridCell).add(newCatchable);
			}
		}

		if (!paintables.containsKey(newPaintable.getType())) {
			paintables.put(newPaintable.getType(), new ArrayList<Paintable>());
		}

		paintables.get(newPaintable.getType()).add(newPaintable);
		repaint();
	}

	public void removePaintable(Paintable paintable) {
		if (paintable instanceof Catchable) {
			Catchable catchable = (Catchable) paintable;
			for (CoordinatePair cp : catchable.getCatchPoints()) {
				CoordinatePair gridCell = new CoordinatePair(cp.getX()
						/ GRID_SIZE, cp.getY() / GRID_SIZE);
				gridMap.get(gridCell).remove(catchable);
			}
		}
		if (paintables.containsKey(paintable.getType())) {
			paintables.get(paintable.getType()).remove(paintable);
		}
		repaint();
	}

	private void changeCurrentCatchable(Catchable catchable) {
		if (currentCaughtCatchable != catchable) {
			if (currentCaughtCatchable != null) {
				currentCaughtCatchable.setCaught(false);
			}
			currentCaughtCatchable = catchable;
			if (currentCaughtCatchable != null)
				currentCaughtCatchable.setCaught(true);
			repaint();
		}
	}

	// ------------------------------------------------------------------------

	private class JMyToolBar extends JToolBar {

		private ButtonGroup buttons;

		private JToggleButton createButton;

		private JToggleButton moveButton;

		private JToggleButton removeButton;

		private JButton colorChangeButton;

		private JMyToolBar() {
			setFloatable(false);
			buttons = new ButtonGroup();
			createButton = new JToggleButton();
			moveButton = new JToggleButton();
			removeButton = new JToggleButton();
			colorChangeButton = new JButton(Icons.RED_DOT);

			createButton.setAction(new AbstractAction("add") {

				@Override
				public void actionPerformed(ActionEvent e) {
					state = State.Add;
					drawingArea.setCursor(new Cursor(Cursor.CROSSHAIR_CURSOR));
					drawingArea.removeMouseMotionListener(mouseAdapter);
				}
			});
			moveButton.setAction(new AbstractAction("move") {

				@Override
				public void actionPerformed(ActionEvent e) {
					state = State.Move;
					drawingArea.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
					drawingArea.removeMouseMotionListener(mouseAdapter);
					drawingArea.addMouseMotionListener(mouseAdapter);
				}
			});
			removeButton.setAction(new AbstractAction("remove") {

				@Override
				public void actionPerformed(ActionEvent e) {
					state = State.Remove;
					drawingArea.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
					drawingArea.removeMouseMotionListener(mouseAdapter);
					drawingArea.addMouseMotionListener(mouseAdapter);
				}
			});

			colorChangeButton.setAction(new AbstractAction("", Icons.RED_DOT) {

				@Override
				public void actionPerformed(ActionEvent e) {
					if (currentDotColor == Dot.DotColor.RED) {
						currentDotColor = Dot.DotColor.BLUE;
						colorChangeButton.setIcon(Icons.BLUE_DOT);
					} else {
						currentDotColor = Dot.DotColor.RED;
						colorChangeButton.setIcon(Icons.RED_DOT);
					}
				}
			});

			buttons.add(createButton);
			buttons.add(moveButton);
			buttons.add(removeButton);

			add(createButton);
			add(moveButton);
			add(removeButton);
			addSeparator();
			add(colorChangeButton);
		}
	}

	// ------------------------------------------------------------------------

	private class MyMouseListener extends MouseAdapter {

		@Override
		public void mousePressed(MouseEvent e) {
			if (e.getButton() == MouseEvent.BUTTON1 && state != null) {
				switch (state) {
				case Add:
					addPaintable(new Dot(e.getX(), e.getY(), currentDotColor));
					break;
				case Move:
					if (currentCaughtCatchable != null) {
						CoordinatePair cp = currentCaughtCatchable
								.getDragPosition();
						Point p = new Point(cp.getX(), cp.getY());
						SwingUtilities.convertPointToScreen(p, drawingArea);
						robot.mouseMove((int) p.getX(), (int) p.getY());

						catchableDragged = currentCaughtCatchable;
					}
				case Remove:
					if (currentCaughtCatchable != null) {
						removePaintable(currentCaughtCatchable);
						currentCaughtCatchable = null;
					}
					break;
				default:
				}
			}
		}

		@Override
		public void mouseReleased(MouseEvent e) {
			if (state == State.Move && catchableDragged != null) {
				catchableDragged.moveTo(e.getX(), e.getY());
				addPaintable(catchableDragged);
				catchableDragged = null;

				repaint();
			}
		}

		@Override
		public void mouseMoved(MouseEvent e) {
			CoordinatePair mousePosition = new CoordinatePair(e.getX(),
					e.getY());
			CoordinatePair[] gridCells = new CoordinatePair[4];
			int mainX = (e.getX() + GRID_SIZE / 2) / GRID_SIZE;
			int mainY = (e.getY() + GRID_SIZE / 2) / GRID_SIZE;
			gridCells[0] = new CoordinatePair(mainX, mainY);
			gridCells[1] = new CoordinatePair(mainX - 1, mainY);
			gridCells[2] = new CoordinatePair(mainX, mainY - 1);
			gridCells[3] = new CoordinatePair(mainX - 1, mainY - 1);

			double min = CATCH_RADIUS;
			double dist;
			Catchable closestCatchable = null;

			for (CoordinatePair gridCell : gridCells) {
				Set<Catchable> catchables = gridMap.get(gridCell);
				if (!(catchables == null || catchables.isEmpty())) {
					for (Catchable catchable : catchables) {
						dist = catchable.getDistanceTo(mousePosition);
						if (dist < min) {
							closestCatchable = catchable;
							min = dist;
						}
					}
				}
			}
			changeCurrentCatchable(closestCatchable);
		}

	}
}