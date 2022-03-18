import * as React from 'react';
import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell from '@mui/material/TableCell';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TableRow from '@mui/material/TableRow';
import Paper from '@mui/material/Paper';

function createData(attribute, value) {
  return { attribute, value };
}

export default function SummaryTable(props) {
  const rows = [
    createData('Duration (h:m:s.ms)', props.details.duration),
    createData('Cycles', props.details.cycles),
    createData('Load Scaling (relative to Earth)', props.details.scale),
    createData('Error', props.details.error),
  ];
  return (
    <TableContainer component={Paper} style={{background: '#121212'}}>
      <Table aria-label="simple table">
        <TableHead>
          <TableRow>
            <TableCell style={{color: '#ffffff'}}>Attribute</TableCell>
            <TableCell align="right" style={{color: '#ffffff'}}>Value</TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {rows.map((row) => (
            <TableRow
              key={row.attribute}
              sx={{ '&:last-child td, &:last-child th': { border: 0 } }}
              style={{color: '#ffffff'}}
            >
              <TableCell component="th" scope="row" style={{color: '#ffffff'}}>
                {row.attribute}
              </TableCell>
              <TableCell align="right" style={{color: '#ffffff'}}>{row.value}</TableCell>
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </TableContainer>
  );
}