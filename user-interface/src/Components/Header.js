import React from "react";
import { Link } from "react-router-dom";
import Typography from '@mui/material/Typography';

export default function Header() {
    return(
        <div style={{backgroundColor: '#282828', marginBottom: 20}}>
            <Typography gutterBottom variant="h3" component="div" style={{textAlign: 'center', color: '#ffffff', paddingTop: 10, marginBottom: 5}}>
                Plyo-Metrics
            </Typography>
            <nav
                style={{
                borderBottom: "solid 1px",
                paddingBottom: "1rem",
                textAlign: 'center'
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