import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import jni.JNITest;

public class MainApp extends JFrame {

	private static MainApp	instance;

	public static MainApp getInstance() {
		if (instance == null) {
			instance = new MainApp();
		}
		return instance;
	}

	public static void main(String[] args) {

		SwingUtilities.invokeLater(new Runnable() {

			@Override
			public void run() {
				JNITest.test();
				MainApp.getInstance().startProgram();
			}
		});
	}

	private MainApp() {
		super("Voronoi Editor");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	private void startProgram() {
		setLookAndFeel();
		showGUI();
	}

	private void setLookAndFeel() {
		try {
			Class.forName("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
			UIManager
					.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
		} catch (ClassNotFoundException e) {
			System.out
					.println("ClassNotFoundException: WindowsLookAndFeel not found");
		} catch (InstantiationException | IllegalAccessException
				| UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}
	}

	private void showGUI() {

		// PaintPanel erstellen und in das Fenster legen
		JMalPanel panel = new JMalPanel();
		panel.setPreferredSize(new Dimension(400, 300));
		getContentPane().add(panel);

		// Fenster zentrieren und anzeigen
		pack();
		setLocationRelativeTo(null);
		setVisible(true);
	}
}
