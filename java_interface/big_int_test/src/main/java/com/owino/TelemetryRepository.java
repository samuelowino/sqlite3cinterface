package com.owino;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
public class TelemetryRepository {
    private final String db = "intsTests";
    public void save(Telemetry entity) {
        try (Connection connection = LocalConnector.getConnection(db)) {
            String sql = """
                    INSERT INTO Telemetry (
                          uuid,
                          type,
                          range,
                          model
                      ) VALUES (
                          ?, ?, ?, ?
                      );
                    """;
            var statement = connection.prepareStatement(sql);
            statement.setString(1, entity.uuid());
            statement.setString(2, entity.type());
            statement.setLong(3, entity.range());
            statement.setString(4, entity.model());
            statement.execute();
        } catch (ClassNotFoundException | SQLException ex) {
            System.out.println("TelemetryRepository: Failed to save Telemetry: error: " + ex.getLocalizedMessage());
        }
    }
    public List<Telemetry> findAll() {
        try (Connection connection = LocalConnector.getConnection(db)) {
            List<Telemetry> telemetries = new ArrayList<>();
            String sql = "SELECT * FROM Telemetry;" ;
            var statement = connection.prepareStatement(sql);
            var resultSet = statement.executeQuery();
            while (resultSet.next()) {
                var uuid = resultSet.getString("uuid");
                var type = resultSet.getString("type");
                var range = resultSet.getLong("range");
                var model = resultSet.getString("model");
                var entity = new Telemetry(
                        uuid,
                        type,
                        range,
                        model
                );
                telemetries.add(entity);
            }
            return telemetries;
        } catch (ClassNotFoundException | SQLException ex) {
            System.out.println("TelemetryRepository: Failed to load Telemetry objects error: " + ex.getLocalizedMessage());
            throw new AssertionError(ex);
        }
    }
}
