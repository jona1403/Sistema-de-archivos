import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SeePDFComponent } from './see-pdf.component';

describe('SeePDFComponent', () => {
  let component: SeePDFComponent;
  let fixture: ComponentFixture<SeePDFComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SeePDFComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(SeePDFComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
