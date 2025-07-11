package com.owino;
import lombok.extern.slf4j.Slf4j;
import java.util.UUID;
@Slf4j
class Runner {
    public static void main(String[] args) {
        var schema = new SyncSQLiteSchema();
        schema.prepareSchema();
        var repository = new TelemetryRepository();
        var smallIntRange = new Telemetry(
                UUID.randomUUID().toString(),
                "Wireless",                 // Type
                438764837678L,                     // Range
                "M-KUL-3411"                       // Model
        );
        var bigIntRange = new Telemetry(
                UUID.randomUUID().toString(),
                "Wireless",                 // Type
                -8417369560160395865L,             // Range
                "M-KUL-3411"                       // Model
        );
        repository.save(smallIntRange);
        repository.save(bigIntRange);
        repository.findAll().forEach(System.out::println);
        schema.clearTable("Telemetry");
    }
}