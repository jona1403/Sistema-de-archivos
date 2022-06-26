import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AdminCarpetasArchivosComponent } from './admin-carpetas-archivos.component';

describe('AdminCarpetasArchivosComponent', () => {
  let component: AdminCarpetasArchivosComponent;
  let fixture: ComponentFixture<AdminCarpetasArchivosComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ AdminCarpetasArchivosComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(AdminCarpetasArchivosComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
