package com.owino;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
public class LocalConnector {
    public static Connection getConnection(String db) throws ClassNotFoundException, SQLException {
        Class.forName("org.sqlite.JDBC");
        String database = "jdbc:sqlite:" + db;
        return DriverManager.getConnection(database);
    }
}
