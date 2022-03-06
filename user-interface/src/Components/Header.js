import React from "react";
import { Link } from "react-router-dom";

export default function Header() {
    return(
        <div>
            <h1> Plyo-Metrics </h1>
            <nav
                style={{
                borderBottom: "solid 1px",
                paddingBottom: "1rem",
                }}
            >
                <Link to="/select">Select</Link> |{" "}
                <Link to="/calibrate">Calibrate</Link> |{" "}
                <Link to="/workout">Workout</Link> |{" "}
                <Link to="/summary">Summary</Link>
            </nav>
        </div>
    );
};