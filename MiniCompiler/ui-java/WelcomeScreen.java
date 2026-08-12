import javax.swing.*;
import java.awt.*;

public class WelcomeScreen extends JFrame {

    public WelcomeScreen() {

        setTitle("MiniLang Compiler");
        setSize(900, 600);
        setMinimumSize(new Dimension(760, 560));
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        /*
         * Palette
         */
        Color backgroundTop = new Color(16, 17, 32);
        Color backgroundBottom = new Color(38, 22, 58);
        Color cardColor = new Color(24, 26, 38);
        Color cardBorder = new Color(55, 65, 81);
        Color accentColor = new Color(34, 211, 238);
        Color mutedColor = new Color(156, 163, 175);
        Color teamTextColor = new Color(229, 231, 235);

        /*
         * Diagonal gradient background that fills the whole window.
         */
        UIComponents.GradientPanel background =
                new UIComponents.GradientPanel(backgroundTop, backgroundBottom);

        background.setLayout(new GridBagLayout());

        /*
         * Centered rounded "card" holding all of the welcome content.
         */
        JPanel card = new JPanel();
        card.setLayout(new BoxLayout(card, BoxLayout.Y_AXIS));
        card.setBackground(cardColor);
        card.setBorder(BorderFactory.createCompoundBorder(
                new UIComponents.RoundedLineBorder(cardBorder, 1, 26),
                BorderFactory.createEmptyBorder(46, 60, 42, 60)
        ));

        JLabel badge = new JLabel("MINILANG COMPILER");
        badge.setFont(new Font("Segoe UI", Font.BOLD, 13));
        badge.setForeground(accentColor);
        badge.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel title = new JLabel("Welcome to Our Mini Compiler");
        title.setFont(new Font("Segoe UI", Font.BOLD, 30));
        title.setForeground(Color.WHITE);
        title.setAlignmentX(Component.CENTER_ALIGNMENT);
        title.setHorizontalAlignment(SwingConstants.CENTER);

        JLabel subtitle = new JLabel("Compiler Design Project");
        subtitle.setFont(new Font("Segoe UI", Font.PLAIN, 16));
        subtitle.setForeground(mutedColor);
        subtitle.setAlignmentX(Component.CENTER_ALIGNMENT);

        /*
         * Thin gradient accent bar used as a decorative divider.
         */
        UIComponents.GradientPanel divider =
                new UIComponents.GradientPanel(accentColor, cardColor);
        divider.setPreferredSize(new Dimension(220, 3));
        divider.setMaximumSize(new Dimension(220, 3));
        divider.setAlignmentX(Component.CENTER_ALIGNMENT);

        JLabel teamHeading = new JLabel("TEAM");
        teamHeading.setFont(new Font("Segoe UI", Font.BOLD, 12));
        teamHeading.setForeground(mutedColor);
        teamHeading.setAlignmentX(Component.CENTER_ALIGNMENT);

        /*
         * Each team member gets its own centered label, laid out in a
         * lightly bordered "roster" panel instead of a plain text block.
         */
        String[] teamMembers = {
                "Tanjim Tajwar Arnab   \u2022   22701066",
                "Hafiz Hasnat Sifat Jami   \u2022   22701068",
                "Muznabin Ahmed   \u2022   22701069",
                "Monir Hossain   \u2022   21701009"
        };

        JPanel rosterPanel = new JPanel();
        rosterPanel.setLayout(new BoxLayout(rosterPanel, BoxLayout.Y_AXIS));
        rosterPanel.setOpaque(false);
        rosterPanel.setAlignmentX(Component.CENTER_ALIGNMENT);

        for (String member : teamMembers) {
            JLabel memberLabel = new JLabel(member);
            memberLabel.setFont(new Font("Consolas", Font.PLAIN, 15));
            memberLabel.setForeground(teamTextColor);
            memberLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
            memberLabel.setHorizontalAlignment(SwingConstants.CENTER);
            memberLabel.setBorder(BorderFactory.createEmptyBorder(4, 0, 4, 0));
            rosterPanel.add(memberLabel);
        }

        JButton startButton = new UIComponents.RoundedButton(
                "\u25B6   Start Compiler",
                accentColor
        );

        startButton.setForeground(new Color(8, 15, 20));
        startButton.setAlignmentX(Component.CENTER_ALIGNMENT);

        startButton.addActionListener(e -> {
            new CompilerUI();
            dispose();
        });

        JLabel footer = new JLabel("Flex \u00b7 Bison \u00b7 C \u00b7 Java Swing");
        footer.setFont(new Font("Segoe UI", Font.PLAIN, 12));
        footer.setForeground(new Color(107, 114, 128));
        footer.setAlignmentX(Component.CENTER_ALIGNMENT);

        card.add(badge);
        card.add(Box.createRigidArea(new Dimension(0, 14)));
        card.add(title);
        card.add(Box.createRigidArea(new Dimension(0, 8)));
        card.add(subtitle);
        card.add(Box.createRigidArea(new Dimension(0, 22)));
        card.add(divider);
        card.add(Box.createRigidArea(new Dimension(0, 22)));
        card.add(teamHeading);
        card.add(Box.createRigidArea(new Dimension(0, 10)));
        card.add(rosterPanel);
        card.add(Box.createRigidArea(new Dimension(0, 30)));
        card.add(startButton);
        card.add(Box.createRigidArea(new Dimension(0, 18)));
        card.add(footer);

        background.add(card);

        add(background);

        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(WelcomeScreen::new);
    }
}
