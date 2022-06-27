import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
@Component({
  selector: 'app-admin-carpetas-archivos',
  templateUrl: './admin-carpetas-archivos.component.html',
  styleUrls: ['./admin-carpetas-archivos.component.css']
})
export class AdminCarpetasArchivosComponent implements OnInit {

  path = new FormControl('');
  constructor() { }

  ngOnInit(): void {
  }


  open() {
    console.log(this.path.value)
  }


}
