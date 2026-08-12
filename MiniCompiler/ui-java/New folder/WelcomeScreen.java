import javax.swing.*;
import java.awt.*;

public class WelcomeScreen extends JFrame {

    public WelcomeScreen() {

        setTitle("MiniLang Compiler");
        setSize(900, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        JPanel panel = new JPanel();
        panel.setBackground(new Color(20, 20, 35));
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));

        JLabel title = new JLabel("Welcome To Our Mini Compiler");
        title.setFont(new Font("Arial", Font.BOLD, 32));
        title.setForeground(Color.WHITE);
        title.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel subtitle = new JLabel("Compiler Design Project");
        subtitle.setFont(new Font("Arial", Font.PLAIN, 20));
        subtitle.setForeground(Color.CYAN);
        subtitle.setAlignmentX(Component.CENTER_ALIGNMENT);

        JTextArea team = new JTextArea(
                "\nTanjim Tajwar Arnab (22701066)\n" +
                "Hafiz Hasnat Sifat Jami (22701068)\n" +
                "Muznabin Ahmed (22701069)\n" +
                "Monir Hossain (21701009)"
        );

        team.setEditable(false);
        team.setBackground(new Color(20, 20, 35));
        team.setForeground(Color.WHITE);
        team.setFont(new Font("Consolas", Font.PLAIN, 18));
        team.setAlignmentX(Component.CENTER_ALIGNMENT);

        JButton startButton = new JButton("Start Compiler");

        startButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        startButton.setFont(new Font("Arial", Font.BOLD, 18));
        startButton.setBackground(Color.CYAN);

        startButton.addActionListener(e -> {
            new CompilerUI();
            dispose();
        });

        panel.add(Box.createVerticalGlue());
        panel.add(title);
        panel.add(Box.createRigidArea(new Dimension(0, 15)));
        panel.add(subtitle);
        panel.add(Box.createRigidArea(new Dimension(0, 30)));
        panel.add(team);
        panel.add(Box.createRigidArea(new Dimension(0, 30)));
        panel.add(startButton);
        panel.add(Box.createVerticalGlue());

        add(panel);

        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(WelcomeScreen::new);
    }
}