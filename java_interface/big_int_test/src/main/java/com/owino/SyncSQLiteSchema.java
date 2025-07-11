package com.owino;
import lombok.extern.slf4j.Slf4j;
import java.sql.Connection;
import java.sql.SQLException;
@Slf4j
public class SyncSQLiteSchema {
    private final String db = "intsTests";
    public void prepareSchema(){
        try(Connection connection = LocalConnector.getConnection(db)){
            String prepareTableSql = """
                    
                    CREATE TABLE IF NOT EXISTS Telemetry (
                        uuid TEXT PRIMARY KEY,
                        type TEXT,
                        range BIGINT,
                        model TEXT
                    );
                    
                    """;
            connection.setAutoCommit(false);
            var createTableStatement = connection.createStatement();
            createTableStatement.executeUpdate(prepareTableSql);
            connection.commit();
        } catch(SQLException | ClassNotFoundException ex){
            ex.printStackTrace();
        }
    }
    public void clearTable(String tableName) {
        try(Connection connection = LocalConnector.getConnection(db)){
            String sql = "DELETE FROM " + tableName;
            var statement = connection.prepareStatement(sql);
            statement.execute();
            statement.closeOnCompletion();
        } catch(SQLException | ClassNotFoundException ex){
            ex.printStackTrace();
        }
    }
}
