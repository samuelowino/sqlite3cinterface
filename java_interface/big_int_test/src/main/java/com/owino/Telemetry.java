package com.owino;

public record Telemetry(
        String uuid,
        String type,
        long range,
        String model
) {
}
